/*
 *  MIT License
 *  
 *  Copyright (c) 2020 Gaëtan Dezeiraud and Ribault Paul
 *  
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *  
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *  
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */

#include <glm/gtc/matrix_transform.hpp>
#include <libxml/parser.h>
#include <libxml/xmlwriter.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdexcept>

#include "SManager.h"

#ifndef _WIN32
# include <dlfcn.h>
#endif

using namespace ExoRenderer;

typedef IRenderer* (*RENDERERLIB)();
typedef IAudio* (*AUDIOLIB)();

SManager::SManager(void)
: _pRendererLibrary(nullptr), _pAudioLibrary(nullptr), _pCursor(nullptr), _pKeyboard(nullptr), _pMouse(nullptr), _pGamepad(nullptr)
{
    loadSettings();
    loadRendererLib();
    loadAudioLib();
}

SManager::~SManager(void)
{
    unloadRendererLib();
    unloadAudioLib();
}

void SManager::loadRendererLib(void)
{
    if (_pRendererLibrary)
        unloadRendererLib();
    
    _pRendererLibrary = new LibLoader<IRenderer>(settings.libGraphic);
}

void SManager::unloadRendererLib(void)
{
    if (_pRendererLibrary)
        delete _pRendererLibrary;
    _pRendererLibrary = nullptr;
}

void SManager::loadAudioLib(void)
{
    if (_pAudioLibrary)
        unloadAudioLib();
    
    _pAudioLibrary = new LibLoader<IAudio>(settings.libAudio);
}

void SManager::unloadAudioLib(void)
{
    delete _pAudioLibrary;
    _pAudioLibrary = nullptr;
}

void SManager::addAudioSource(const std::shared_ptr<ISound> &sound)
{
    auto source = _pAudioLibrary->getPlugin()->createSource();
    source->setAudio(sound.get());
    source->play();
    
    _globalSources.push_back(source);
}

void SManager::updateAudioSources()
{
    for (unsigned int i = 0; i < _globalSources.size(); i++)
    {
        if (_globalSources[i]->getState() != ISource::SourceState::PLAYING)
        {
            delete _globalSources[i];
            _globalSources.erase(_globalSources.begin() + i);
        }
        else
            _globalSources[i]->streamingUpdate(); // If it's a music
    }
}

void SManager::loadSettings(void)
{
    xmlDocPtr document;
    xmlNodePtr root;
    
    document = xmlParseFile("resources/settings.xml");
    if (document == NULL)
    {
        _log.error << "Settings Document XML invalid" << std::endl;
        updateSettings();
        return;
    }
   
    root = xmlDocGetRootElement(document);
    if (root == NULL)
    {
        _log.warning << "Settings Document XML empty" << std::endl;
        xmlFreeDoc(document);
        updateSettings();
        return;
    }
        
    if (root->type == XML_ELEMENT_NODE && xmlStrcmp(root->name, (const xmlChar*)"settings") == 0)
    {
        for (auto currentNode = root->children; currentNode; currentNode = currentNode->next)
        {
            if (currentNode->type == XML_ELEMENT_NODE) {
                if (xmlStrcmp(currentNode->name, (const xmlChar*)"username") == 0)
                    settings.username = (char*) currentNode->children->content;
                else if (xmlStrcmp(currentNode->name, (const xmlChar*)"width") == 0)
                    settings.width = atof((char*) currentNode->children->content);
                else if (xmlStrcmp(currentNode->name, (const xmlChar*)"height") == 0)
                    settings.height = atof((char*) currentNode->children->content);
                else if (xmlStrcmp(currentNode->name, (const xmlChar*)"windowMode") == 0)
                    settings.windowMode = (WindowMode)atoi((char*) currentNode->children->content);
                else if (xmlStrcmp(currentNode->name, (const xmlChar*)"vsync") == 0)
                    settings.vsync = strcmp((char*) currentNode->children->content, "0") == 0 ? false : true;
                else if (xmlStrcmp(currentNode->name, (const xmlChar*)"fps") == 0)
                    settings.fps = strcmp((char*) currentNode->children->content, "0") == 0 ? false : true;
                else if (xmlStrcmp(currentNode->name, (const xmlChar*)"globalVolume") == 0)
                    settings.globalVolume = atof((char*) currentNode->children->content);
                else if (xmlStrcmp(currentNode->name, (const xmlChar*)"language") == 0)
                    settings.language = (char*) currentNode->children->content;
                else if (xmlStrcmp(currentNode->name, (const xmlChar*)"libGraphic") == 0)
                    settings.libGraphic = (char*) currentNode->children->content;
                else if (xmlStrcmp(currentNode->name, (const xmlChar*)"libAudio") == 0)
                    settings.libAudio = (char*) currentNode->children->content;
                else if (xmlStrcmp(currentNode->name, (const xmlChar*)"servers") == 0)
                {
                    for (auto currentChildNode = currentNode->children; currentChildNode; currentChildNode = currentChildNode->next)
                    {
                        if (currentChildNode->type == XML_ELEMENT_NODE)
                        {
                            std::string name, ip, port;
                            for (auto currentChildOfChildNode = currentChildNode->children; currentChildOfChildNode; currentChildOfChildNode = currentChildOfChildNode->next)
                            {
                                if (xmlStrcmp(currentChildOfChildNode->name, (const xmlChar*)"name") == 0)
                                    name = (char*) currentChildOfChildNode->children->content;
                                else if (xmlStrcmp(currentChildOfChildNode->name, (const xmlChar*)"ip") == 0)
                                    ip = (char*) currentChildOfChildNode->children->content;
                                else if (xmlStrcmp(currentChildOfChildNode->name, (const xmlChar*)"port") == 0)
                                    port = (char*) currentChildOfChildNode->children->content;
                            }
                            settings.servers.push_back({name, ip, port});
                        }
                    }
                }
                else if (xmlStrcmp(currentNode->name, (const xmlChar*)"threads") == 0)
                    settings.threads = std::stoul((char *)currentNode->children->content);
            }
        }
    }
    
    xmlFreeDoc(document);
}

void SManager::updateSettings(void)
{
    xmlTextWriterPtr writer;
    writer = xmlNewTextWriterFilename("resources/settings.xml", 0);
    xmlTextWriterSetIndent(writer, 1);
    xmlTextWriterStartDocument(writer, NULL, "UTF-8", NULL);
    
    xmlTextWriterStartElement(writer, BAD_CAST "settings");
        xmlTextWriterWriteElement(writer, BAD_CAST "username", BAD_CAST settings.username.c_str());
        xmlTextWriterWriteElement(writer, BAD_CAST "width", BAD_CAST std::to_string(settings.width).c_str());
        xmlTextWriterWriteElement(writer, BAD_CAST "height", BAD_CAST std::to_string(settings.height).c_str());
        xmlTextWriterWriteElement(writer, BAD_CAST "windowMode", BAD_CAST std::to_string(settings.windowMode).c_str());
        xmlTextWriterWriteElement(writer, BAD_CAST "vsync", BAD_CAST std::to_string(settings.vsync).c_str());
        xmlTextWriterWriteElement(writer, BAD_CAST "fps", BAD_CAST std::to_string(settings.fps).c_str());
        xmlTextWriterWriteElement(writer, BAD_CAST "globalVolume", BAD_CAST std::to_string(settings.globalVolume).c_str());
        xmlTextWriterWriteElement(writer, BAD_CAST "language", BAD_CAST settings.language.c_str());
        xmlTextWriterWriteElement(writer, BAD_CAST "libGraphic", BAD_CAST settings.libGraphic.c_str());
        xmlTextWriterWriteElement(writer, BAD_CAST "libAudio", BAD_CAST settings.libAudio.c_str());

        xmlTextWriterStartElement(writer, BAD_CAST "servers");
            for(unsigned int i = 0; i < settings.servers.size(); i++)
            {
                xmlTextWriterStartElement(writer, BAD_CAST "server");
                    xmlTextWriterWriteElement(writer, BAD_CAST "name", BAD_CAST settings.servers[i].name.c_str());
                    xmlTextWriterWriteElement(writer, BAD_CAST "ip", BAD_CAST settings.servers[i].ip.c_str());
                    xmlTextWriterWriteElement(writer, BAD_CAST "port", BAD_CAST settings.servers[i].port.c_str());
                xmlTextWriterEndElement(writer);
            }
        xmlTextWriterEndElement(writer);
    xmlTextWriterEndElement(writer);
    
    xmlTextWriterEndDocument(writer);
    xmlFreeTextWriter(writer);
}

// Setters
void SManager::setCursor(ICursor* cursor)
{
    _pCursor = cursor;
}

void SManager::setKeyboard(IKeyboard* keyboard)
{
    _pKeyboard = keyboard;
}

void SManager::setMouse(IMouse* mouse)
{
    _pMouse = mouse;
}

void SManager::setGamepadManager(IGamepadManager* gamepad)
{
    _pGamepad = gamepad;
}

// Getters
IRenderer* SManager::getRenderer(void)
{
    return _pRendererLibrary->getPlugin();
}

ICursor* SManager::getCursor(void)
{
    return _pCursor;
}

IKeyboard* SManager::getKeyboard(void)
{
    return _pKeyboard;
}

IMouse* SManager::getMouse(void)
{
    return _pMouse;
}

IGamepadManager* SManager::getGamepadManager(void)
{
    return _pGamepad;
}

IAudio* SManager::getAudio(void)
{
    return _pAudioLibrary->getPlugin();
}
