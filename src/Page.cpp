/*
 * File: Page.cpp
 * Created: 4th April 2020 7:10 pm
 * Author: Ribault Paul (pribault.dev@gmail.com)
 * 
 * Last Modified: 4th April 2020 7:10 pm
 * Modified By: Ribault Paul (pribault.dev@gmail.com)
 * 
 * Copyright © 2020 Ribault Paul. All rights reserved.
 */

#include "Page.h"
#include "SceneStateMachine.h"
#include "ResourceManager.h"

#include "Log.h"

using namespace	ExoEngine;
using namespace	ExoRenderer;

/*
	CENTER,
	CENTER_LEFT,
	CENTER_RIGHT,
	TOP_LEFT,
	TOP_RIGHT,
	TOP_CENTER,
	BOTTOM_LEFT,
	BOTTOM_RIGHT,
	BOTTOM_CENTER
*/

static AnchorPoint	stringToAnchor(const std::string& string)
{
	static const std::map<std::string, AnchorPoint>	map = {
		std::make_pair("center", AnchorPoint::CENTER),
		std::make_pair("center_left", AnchorPoint::CENTER_LEFT),
		std::make_pair("center_right", AnchorPoint::CENTER_RIGHT),
		std::make_pair("top_left", AnchorPoint::TOP_LEFT),
		std::make_pair("top_right", AnchorPoint::TOP_RIGHT),
		std::make_pair("top_center", AnchorPoint::TOP_CENTER),
		std::make_pair("bottom_left", AnchorPoint::BOTTOM_LEFT),
		std::make_pair("bottom_right", AnchorPoint::BOTTOM_RIGHT),
		std::make_pair("bottom_center", AnchorPoint::BOTTOM_CENTER)
	};
	std::map<std::string, AnchorPoint>::const_iterator	iterator;

	iterator = map.find(string);
	if (iterator != map.end())
		return (iterator->second);
	_log.warning << "unknown anchor point '" << string << "'" << std::endl;
	return (AnchorPoint::CENTER);
}

Page::Page(SceneStateMachine &stateMachine, const std::string& name, const std::string& file) :
	Scene(stateMachine, name),
	_resourceManager(((SceneStateMachine&)getStateMachine()).getResourceManager())
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

	if (root->type == XML_ELEMENT_NODE && !xmlStrcmp(root->name, (const xmlChar*)"page"))
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

Page::~Page(void)
{
	for (const std::pair<std::string, IWidget*>& widget : _widgets)
	{
		delete widget.second;
	}
}

void	Page::load(void)
{
	_log.meeseeks << __PRETTY_FUNCTION__ << " - " << __LINE__ << std::endl;
	for (const std::pair<std::string, IWidget*>& widget : _widgets)
	{
		_resourceManager->getRenderer()->add(widget.second);
	}
}

void	Page::unload(void)
{
	_log.meeseeks << __PRETTY_FUNCTION__ << " - " << __LINE__ << std::endl;
	for (const std::pair<std::string, IWidget*>& widget : _widgets)
	{
		_resourceManager->getRenderer()->remove(widget.second);
	}
}

void	Page::draw(void)
{
	for (const std::pair<std::string, IWidget*>& widget : _widgets)
	{
	}
}

void	Page::handleEvents(void)
{
	IRenderer*	renderer = _resourceManager->getRenderer();

	for (const std::pair<std::string, IWidget*>& widget : _widgets)
	{
		widget.second->update(renderer->getMouse(), renderer->getKeyboard(), renderer->getGamepadManager()->getGamepad(0), renderer->getNavigationType());
	}
}

IWidget*	Page::loadImage(const std::map<std::string, std::string>& properties)
{
	IImage*			image;
	std::string		texture;
	std::string		x("0");
	std::string		y("0");
	std::string		width("100");
	std::string		height("100");
	std::string		anchor;

	std::map<std::string, std::string>::const_iterator	iterator;

	try
	{
		texture = properties.at("texture");
	}
	catch (const std::exception &)
	{
		throw (std::invalid_argument("no texture for image"));
	}

	iterator = properties.find("x");
	if (iterator != properties.end())
		x = iterator->second;

	iterator = properties.find("y");
	if (iterator != properties.end())
		y = iterator->second;

	iterator = properties.find("width");
	if (iterator != properties.end())
		width = iterator->second;

	iterator = properties.find("height");
	if (iterator != properties.end())
		height = iterator->second;

	iterator = properties.find("anchor");
	if (iterator != properties.end())
		anchor = iterator->second;

	image = _resourceManager->getRenderer()->createImage(_resourceManager->get<ITexture>(properties.at("texture")));

	if (!anchor.empty())
		image->setAnchor(stringToAnchor(anchor));
	else
		image->setPosition(std::stof(x), std::stof(y));
	image->setSize(std::stof(width), std::stof(height));
	return (image);
}

IWidget*	Page::loadButton(const std::map<std::string, std::string>& properties)
{
	IButton*		button;
	std::string		texture;
	std::string		x("0");
	std::string		y("0");
	std::string		width("100");
	std::string		height("100");
	std::string		anchor;
	std::string		text;
	std::string		font;

	std::map<std::string, std::string>::const_iterator	iterator;

	try
	{
		texture = properties.at("texture");
	}
	catch (const std::exception &)
	{
		throw (std::invalid_argument("no texture for image"));
	}

	iterator = properties.find("x");
	if (iterator != properties.end())
		x = iterator->second;

	iterator = properties.find("y");
	if (iterator != properties.end())
		y = iterator->second;

	iterator = properties.find("width");
	if (iterator != properties.end())
		width = iterator->second;

	iterator = properties.find("height");
	if (iterator != properties.end())
		height = iterator->second;

	iterator = properties.find("anchor");
	if (iterator != properties.end())
		anchor = iterator->second;

	iterator = properties.find("text");
	if (iterator != properties.end())
		text = iterator->second;

	iterator = properties.find("font");
	if (iterator != properties.end())
		font = iterator->second;

	button = _resourceManager->getRenderer()->createButton(_resourceManager->get<ITexture>(properties.at("texture")));

	if (!anchor.empty())
		button->setAnchor(stringToAnchor(anchor));
	else
		button->setPosition(std::stof(x), std::stof(y));
	button->setSize(std::stof(width), std::stof(height));
	if (!font.empty())
		button->getLabel()->setFont(_resourceManager->get<Font>(font));
	if (!text.empty())
		button->getLabel()->setText(text);
	return (button);
}

IWidget*	Page::loadCheckbox(const std::map<std::string, std::string>& properties)
{
	ICheckbox*		checkbox;
	std::string		texture;
	std::string		x("0");
	std::string		y("0");
	std::string		width("100");
	std::string		height("100");
	std::string		anchor;

	std::map<std::string, std::string>::const_iterator	iterator;

	try
	{
		texture = properties.at("texture");
	}
	catch (const std::exception &)
	{
		throw (std::invalid_argument("no texture for image"));
	}

	iterator = properties.find("x");
	if (iterator != properties.end())
		x = iterator->second;

	iterator = properties.find("y");
	if (iterator != properties.end())
		y = iterator->second;

	iterator = properties.find("width");
	if (iterator != properties.end())
		width = iterator->second;

	iterator = properties.find("height");
	if (iterator != properties.end())
		height = iterator->second;

	iterator = properties.find("anchor");
	if (iterator != properties.end())
		anchor = iterator->second;

	checkbox = _resourceManager->getRenderer()->createCheckbox(_resourceManager->get<ITexture>(properties.at("texture")));

	if (!anchor.empty())
		checkbox->setAnchor(stringToAnchor(anchor));
	else
		checkbox->setPosition(std::stof(x), std::stof(y));
	checkbox->setSize(std::stof(width), std::stof(height));
	return (checkbox);
}

IWidget*	Page::loadInput(const std::map<std::string, std::string>& properties)
{
	IInput*			input;
	std::string		texture;
	std::string		x("0");
	std::string		y("0");
	std::string		width("100");
	std::string		height("100");
	std::string		anchor;
	std::string		text;
	std::string		font;

	std::map<std::string, std::string>::const_iterator	iterator;

	try
	{
		texture = properties.at("texture");
	}
	catch (const std::exception &)
	{
		throw (std::invalid_argument("no texture for image"));
	}

	iterator = properties.find("x");
	if (iterator != properties.end())
		x = iterator->second;

	iterator = properties.find("y");
	if (iterator != properties.end())
		y = iterator->second;

	iterator = properties.find("width");
	if (iterator != properties.end())
		width = iterator->second;

	iterator = properties.find("height");
	if (iterator != properties.end())
		height = iterator->second;

	iterator = properties.find("anchor");
	if (iterator != properties.end())
		anchor = iterator->second;

	iterator = properties.find("text");
	if (iterator != properties.end())
		text = iterator->second;

	iterator = properties.find("font");
	if (iterator != properties.end())
		font = iterator->second;

	input = _resourceManager->getRenderer()->createInput(_resourceManager->get<ITexture>(properties.at("texture")));

	if (!anchor.empty())
		input->setAnchor(stringToAnchor(anchor));
	else
		input->setPosition(std::stof(x), std::stof(y));
	input->setSize(std::stof(width), std::stof(height));
	if (!font.empty())
		input->setFont(_resourceManager->get<Font>(font));
	if (!text.empty())
		input->setText(text);
	input->setColor(0, 0, 0);
	return (input);
}

IWidget*	Page::loadSelect(const std::map<std::string, std::string>& properties)
{
	return nullptr;
}

IWidget*	Page::loadSlider(const std::map<std::string, std::string>& properties)
{
	return nullptr;
}

IWidget*	Page::loadSpinner(const std::map<std::string, std::string>& properties)
{
	return nullptr;
}

IWidget*	Page::loadView(const std::map<std::string, std::string>& properties)
{
	return nullptr;
}

void	Page::load(xmlNodePtr node, IWidget* parentWidget)
{
	static const std::pair<std::string, t_widget_loader>	loaders[] = {
		{"Image", &Page::loadImage},
		{"Button", &Page::loadButton},
		{"Checkbox", &Page::loadCheckbox},
		{"Input", &Page::loadInput},
		{"Select", &Page::loadSelect},
		{"Slider", &Page::loadSlider},
		{"Spinner", &Page::loadSpinner},
		{"View", &Page::loadView}
	};
	std::map<std::string, std::string>	properties;
	std::string							name;
	std::string							type;
	IWidget*							widget;

	getProperties(node, properties);

	type = std::string((const char*)node->name);
	std::map<std::string, std::string>::iterator nameIterator = properties.find("name");
	if (nameIterator == properties.end())
		throw (std::invalid_argument("widget '" + type + "' without name"));
	name = nameIterator->second;
	widget = nullptr;
	for (size_t index = 0; index < sizeof(loaders) / sizeof(loaders[0]); index++)
	{
		if (type == loaders[index].first)
		{
			widget = (this->*loaders[index].second)(properties);
			break;
		}
	}
	if (!widget)
		throw (std::invalid_argument("unknown widget type '" + type + "'"));

	if (_widgets.find(name) == _widgets.end())
	{
		_widgets[name] = widget;
	}
	else
	{
		_log.warning << "widget '" << name << "' already exists" << std::endl;
		delete widget;
		return ;
	}

	//	load childs
	for (xmlNodePtr currentNode = node->children; currentNode; currentNode = currentNode->next)
	{
		switch (currentNode->type)
		{
			case XML_ELEMENT_NODE:
			{
				load(currentNode, widget);
				break;
			}
			default:
			{
				break;
			}
		}
	}
}

void	Page::getProperties(xmlNodePtr node, std::map<std::string, std::string>& properties)
{
	xmlNodePtr	content;
	std::string	name;
	std::string	value;

	for (xmlAttrPtr property = node->properties; property; property = property->next)
	{
		name = std::string((const char*)property->name);
		content = property->children;
		if (content && content->content)
		{
			value = std::string((const char*)content->content);
		}
		else
		{
			value = "";
		}
		properties[name] = value;
	}
}
