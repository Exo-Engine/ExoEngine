/*
 *	MIT License
 *
 *	Copyright (c) 2020 Gaëtan Dezeiraud and Ribault Paul
 *
 *	Permission is hereby granted, free of charge, to any person obtaining a copy
 *	of this software and associated documentation files (the "Software"), to deal
 *	in the Software without restriction, including without limitation the rights
 *	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *	copies of the Software, and to permit persons to whom the Software is
 *	furnished to do so, subject to the following conditions:
 *
 *	The above copyright notice and this permission notice shall be included in all
 *	copies or substantial portions of the Software.
 *
 *	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *	SOFTWARE.
 */

#include "Engine.h"
#include "Utils.h"

#include "LibLoader.h"

using namespace	ExoEngine;
using namespace	ExoRenderer;
using namespace	ExoAudio;

Engine::Engine(void) :
	_resourceManager(nullptr),
	_settingsManager(nullptr),
	_rendererPlugin(nullptr),
	_audioPlugin(nullptr)
{
	_settingsManager = new SettingsManager();
}

Engine::Engine(const std::string& settingsFile) : Engine()
{
	initialize(settingsFile);
}

Engine::~Engine(void)
{
	if (_resourceManager)
		delete _resourceManager;
	if (_settingsManager)
		delete _settingsManager;
	if (_rendererPlugin)
		delete _rendererPlugin;
	if (_audioPlugin)
		delete _audioPlugin;
}

void					Engine::initialize(const std::string& settingsFile)
{
	std::string	path;
	Setting*	rendererLibSetting;
	Setting*	audioLibSetting;

	path = getPath(settingsFile);
	_settingsManager->load(settingsFile);
	try
	{
		rendererLibSetting = _settingsManager->get("libGraphic");
	}
	catch (const std::exception&)
	{
		_log.error << "missing libRenderer in settings file '" << settingsFile << "'" << std::endl;
	}
	try
	{
		audioLibSetting = _settingsManager->get("libAudio");
	}
	catch (const std::exception&)
	{
		_log.error << "missing libAudio in settings file '" << settingsFile << "'" << std::endl;
	}
	_rendererPlugin = new LibLoader<IRenderer>(path + rendererLibSetting->getValue());
	_audioPlugin = new LibLoader<IAudio>(path + audioLibSetting->getValue());
	_resourceManager = new ResourceManager(getRenderer(), getAudio());
}

ResourceManager*		Engine::getResourceManager(void) const
{
	return (_resourceManager);
}

SettingsManager*		Engine::getSettingsManager(void) const
{
	return (_settingsManager);
}

ExoRenderer::IRenderer*	Engine::getRenderer(void) const
{
	return (_rendererPlugin->getPlugin());
}

ExoAudio::IAudio*		Engine::getAudio(void) const
{
	return (_audioPlugin->getPlugin());
}
