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

#include "SettingsManager.h"

#include "Log.h"
#include <vector>

using namespace	ExoEngine;

Setting::Setting(const std::string& name) : _name(name)
{
}

Setting::~Setting(void)
{
}

void				Setting::setName(const std::string& name)
{
	_name = name;
}

const std::string&	Setting::getName(void) const
{
	return (_name);
}

void				Setting::addProperty(const std::string& name, const std::string& value)
{
	_properties[name] = value;
}

const std::string&	Setting::getProperty(const std::string& name)
{
	return (_properties.at(name));
}

void				Setting::addChild(Setting* setting)
{
	_childs.push_back(setting);
}

size_t				Setting::getNbChilds(void) const
{
	return (_childs.size());
}

Setting*			Setting::getChild(size_t index)
{
	return (_childs[index]);
}

void				Setting::setValue(const std::string& value)
{
	_value = value;
}

const std::string&	Setting::getValue(void) const
{
	return (_value);
}

SettingsManager::SettingsManager(void)
{
}

SettingsManager::~SettingsManager(void)
{
}

void		SettingsManager::load(const std::string& file)
{
	xmlDocPtr	doc;
	xmlNodePtr	root;

	doc = xmlParseFile(file.c_str());
	if (!doc)
		throw (std::invalid_argument("cannot read xml file '" + file + "'"));
	root = xmlDocGetRootElement(doc);
	if (!root)
	{
		xmlFreeDoc(doc);
		throw (std::invalid_argument("cannot get root node on '" + file + "'"));
	}

	if (root->type == XML_ELEMENT_NODE && !xmlStrcmp(root->name, (const xmlChar*)"settings"))
	{
		for (xmlNodePtr currentNode = root->children; currentNode; currentNode = currentNode->next)
		{
			if (currentNode->type == XML_ELEMENT_NODE)
			{
				load(currentNode, nullptr);
			}
		}
	}

	xmlFreeDoc(doc);
}

void		SettingsManager::unload(void)
{
}

Setting*	SettingsManager::get(const std::string& setting)
{
	return (_settings.at(setting));
}

void		SettingsManager::load(xmlNodePtr node, Setting* parentSetting)
{
	Setting	*setting;

	//	create setting object
	setting = new Setting(std::string((const char*)node->name));
	if (!parentSetting)
		_settings[setting->getName()] = setting;
	else
		parentSetting->addChild(setting);

	//	add properties
	for (xmlAttrPtr property = node->properties; property; property = property->next)
	{
		setting->addProperty(std::string((const char*)property->name), std::string((const char*)property->children->content));
	}

	//	load childs
	for (xmlNodePtr currentNode = node->children; currentNode; currentNode = currentNode->next)
	{
		switch (currentNode->type)
		{
			case XML_ELEMENT_NODE:
			{
				load(currentNode, setting);
				break;
			}
			case XML_TEXT_NODE:
			{
				if (setting && setting->getValue().empty())
					setting->setValue(std::string((const char*)currentNode->content));
				break;
			}
			default:
			{
				break;
			}
		}
	}
}
