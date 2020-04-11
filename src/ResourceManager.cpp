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

#include "ResourceManager.h"

#include "Utils.h"

using namespace	ExoEngine;
using namespace ExoRenderer;
using namespace	ExoAudio;

static std::vector<std::string>	explode(const std::string &str, const std::string &sep)
{
	std::vector<std::string>	vector;
	size_t						i, j;

	for (i = 0; i < str.length(); )
	{
		while (i < str.length() && sep.find(str[i]) != std::string::npos)
			i++;
		for (j = i; j < str.length() && sep.find(str[j]) == std::string::npos; j++)
			;
		if (j != i)
		{
			vector.push_back(std::string(str, i, j - i));
			i = j;
		}
	}
	return (vector);
}

ResourceManager::ResourceManager(ExoRenderer::IRenderer* renderer, IAudio* audio) : _renderer(renderer), _audio(audio)
{
}

ResourceManager::~ResourceManager(void)
{
	_resources.clear();
}

ExoRenderer::IRenderer*	ResourceManager::getRenderer(void) const
{
	return (_renderer);
}

const std::shared_ptr<IResource>& ResourceManager::get(const std::string& name)
{
	auto res = _resources.find(name);
	if (res != _resources.end())
		return (res->second);
	throw (std::invalid_argument("resource '" + name + "' doesn't exists"));
}

const std::string	&ResourceManager::getProperty(const std::string &name)
{
	return (_properties.at(name));
}

void	ResourceManager::loadFont(const std::string &relativePath, xmlNodePtr node)
{
	xmlChar *name = xmlGetProp(node, (const xmlChar *)"name");
	xmlChar *path = xmlGetProp(node, (const xmlChar *)"path");
	xmlChar *texture = xmlGetProp(node, (const xmlChar *)"texture");

	if (!name)
		_log.warning << "font without name" << std::endl;
	if (!path)
		_log.warning << "font without path" << std::endl;
	if (!texture)
		_log.warning << "font without texture" << std::endl;
	if (name && path)
		add((char *)name, std::shared_ptr<Font>(
			new Font(std::shared_ptr<FntLoader>(new FntLoader((relativePath + (char *)path).c_str())), std::shared_ptr<ITexture>(_renderer->createTexture((relativePath + (char *)texture).c_str())))));
}

void	ResourceManager::loadTexture(const std::string &relativePath, xmlNodePtr node)
{
	xmlChar *name = xmlGetProp(node, (const xmlChar *)"name");
	xmlChar *path = xmlGetProp(node, (const xmlChar *)"path");

	if (!name)
		_log.warning << "texture without name" << std::endl;
	if (!path)
		_log.warning << "texture without path" << std::endl;
	if (name && path)
		add((char *)name, std::shared_ptr<ITexture>(_renderer->createTexture((relativePath + (char *)path).c_str())));
}

void	ResourceManager::loadArrayTexture(const std::string &relativePath, xmlNodePtr node)
{
	xmlChar					 *name = xmlGetProp(node, (const xmlChar *)"name");
	xmlChar					 *width = xmlGetProp(node, (const xmlChar *)"width");
	xmlChar					 *height = xmlGetProp(node, (const xmlChar *)"height");
	xmlChar					 *filter = xmlGetProp(node, (const xmlChar *)"filter");
	xmlChar					 *content = xmlNodeGetContent(node);
	std::vector<std::string>	textures;

	if (!name)
		_log.warning << "array texture without name" << std::endl;
	if (!width)
		_log.warning << "array texture without width" << std::endl;
	if (!height)
		_log.warning << "array texture without height" << std::endl;
	if (!content)
		_log.warning << "array texture without content" << std::endl;
	textures = explode((char *)content, " \t\n");
	for (auto texture = textures.begin(); texture != textures.end(); texture++)
		(*texture) = relativePath + (*texture);
	if (name && width && height && content)
		add((char *)name, std::shared_ptr<IArrayTexture>(_renderer->createArrayTexture(std::stoi((char *)width), std::stoi((char *)height), textures, strcmp((char*)filter, "nearest") == 0 ? TextureFilter::NEAREST : TextureFilter::LINEAR)));
}

void	ResourceManager::loadSound(const std::string &relativePath, xmlNodePtr node)
{
	xmlChar *name = xmlGetProp(node, (const xmlChar *)"name");
	xmlChar *path = xmlGetProp(node, (const xmlChar *)"path");

	if (!name)
		_log.warning << "sound without name" << std::endl;
	if (!path)
		_log.warning << "sound without path" << std::endl;
	if (name && path)
		add((char *)name, std::shared_ptr<ISound>(_audio->createSound((relativePath + (char *)path).c_str())));
}

void	ResourceManager::loadSubResource(const std::string &relativePath, xmlNodePtr node)
{
	ResourceManager	*resourceManager;
	xmlChar *name = xmlGetProp(node, (const xmlChar *)"name");
	xmlChar *path = xmlGetProp(node, (const xmlChar *)"path");

	if (!name)
		_log.warning << "sound without name" << std::endl;
	if (!path)
		_log.warning << "sound without path" << std::endl;
	if (name && path)
	{
		resourceManager = new ResourceManager(_renderer, _audio);
		resourceManager->load((relativePath + (char *)path).c_str());
		add((char *)name, std::shared_ptr<ResourceManager>(resourceManager));
	}
}

void	ResourceManager::loadHitboxes(const std::string &path, xmlNodePtr node)
{
	std::vector<hitbox> hitboxesVector;

	for (auto currentNode = node->children; currentNode; currentNode = currentNode->next)
	{
		if (currentNode->type == XML_ELEMENT_NODE)
		{
			if (!xmlStrcmp(currentNode->name, (const xmlChar*)"hitbox"))
			{
				xmlChar *centerX = xmlGetProp(currentNode, (const xmlChar *)"centerX");
				xmlChar *centerY = xmlGetProp(currentNode, (const xmlChar *)"centerY");
				xmlChar *width = xmlGetProp(currentNode, (const xmlChar *)"width");
				xmlChar *height = xmlGetProp(currentNode, (const xmlChar *)"height");

				if (!centerX)
					_log.warning << "hitbox without centerX" << std::endl;
				if (!centerY)
					_log.warning << "hitbox without centerY" << std::endl;
				if (!width)
					_log.warning << "hitbox without width" << std::endl;
				if (!height)
					_log.warning << "hitbox without height" << std::endl;
				if (centerX && centerY && width && height)
					hitboxesVector.push_back({std::stof((char *)centerX), std::stof((char *)centerY), std::stof((char *)width), std::stof((char *)height)});
			}
			else
				_log.warning << "unknown hitbox type '" << currentNode->name << "'" << std::endl;
		}
	}

	if (hitboxesVector.size() > 0)
		add("hitboxes", std::shared_ptr<hitboxes>(new hitboxes(hitboxesVector)));
}

void	ResourceManager::loadProperties(xmlNodePtr node)
{
	xmlAttr *attribute;

	for (attribute = node->properties; attribute; attribute = attribute->next)
	{
		_properties[std::string((char *)attribute->name)] = std::string((char *)attribute->children->content);
		_log.debug << "property " << attribute->name << "=" << attribute->children->content << std::endl;
	}
}

void	ResourceManager::load(const std::string &file)
{
	std::string path;
	xmlDocPtr	doc;
	xmlNodePtr	root;

	path = getPath(file);
	doc = xmlParseFile(file.c_str());
	if (!doc)
		throw (std::invalid_argument("cannot read xml file '" + file + "'"));
	root = xmlDocGetRootElement(doc);
	if (!root)
	{
		xmlFreeDoc(doc);
		throw (std::invalid_argument("cannot get root node on '" + file + "'"));
	}

	if (root->type == XML_ELEMENT_NODE && !xmlStrcmp(root->name, (const xmlChar*)"resources"))
	{
		loadProperties(root);
		for (auto currentNode = root->children; currentNode; currentNode = currentNode->next)
		{
			if (currentNode->type == XML_ELEMENT_NODE)
			{
				if (!xmlStrcmp(currentNode->name, (const xmlChar*)"texture"))
					loadTexture(path, currentNode);
				else if (!xmlStrcmp(currentNode->name, (const xmlChar*)"arrayTexture"))
					loadArrayTexture(path, currentNode);
				else if (!xmlStrcmp(currentNode->name, (const xmlChar*)"sound"))
					loadSound(path, currentNode);
				else if (!xmlStrcmp(currentNode->name, (const xmlChar*)"font"))
					loadFont(path, currentNode);
				else if (!xmlStrcmp(currentNode->name, (const xmlChar*)"sub-resource"))
					loadSubResource(path, currentNode);
				else if (!xmlStrcmp(currentNode->name, (const xmlChar*)"hitboxes"))
					loadHitboxes(path, currentNode);
				else
					_log.warning << "unknown resource type '" << currentNode->name << "'" << std::endl;
			}
		}
	}

	xmlFreeDoc(doc);
}
