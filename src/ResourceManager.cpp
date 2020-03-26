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
#include "SManager.h"

using namespace	ExoEngine;
using namespace ExoRenderer;

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

static std::string	getPath(const std::string &file)
{
	size_t	i;

	for (i = file.length() - 1; i != (size_t)-1 && file[i] != '/'; i--)
		;
	if (i)
		return (std::string(file, 0, i + 1));
	else
		return ("");
}

ResourceManager::ResourceManager(void)
{	}

ResourceManager::~ResourceManager(void)
{
	for (auto resource = _resources.begin(); resource != _resources.end(); resource++)
		delete resource->second;
	_resources.clear();
}

std::shared_ptr<IResource> ResourceManager::get(const std::string& name)
{
	auto res = _resources.find(name);
	if (res != _resources.end())
		return (res->second->get());

	return nullptr;
}

const std::string	&ResourceManager::getProperty(const std::string &name)
{
	return (_properties.at(name));
}

void	ResourceManager::add(const std::string &name, ILoader *loader)
{
	_resources[name] = loader;
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
		add((char *)name, new fontLoader((relativePath + (char *)path).c_str(), (relativePath + (char *)texture).c_str()));
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
		add((char *)name, new textureLoader((relativePath + (char *)path).c_str()));
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
		add((char *)name, new arrayTextureLoader(std::stoi((char *)width), std::stoi((char *)height), textures, strcmp((char*)filter, "nearest") == 0 ? TextureFilter::NEAREST : TextureFilter::LINEAR));
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
		add((char *)name, new soundLoader((relativePath + (char *)path).c_str()));
}

void	ResourceManager::loadSubResource(const std::string &relativePath, xmlNodePtr node)
{
	xmlChar *name = xmlGetProp(node, (const xmlChar *)"name");
	xmlChar *path = xmlGetProp(node, (const xmlChar *)"path");

	if (!name)
		_log.warning << "sound without name" << std::endl;
	if (!path)
		_log.warning << "sound without path" << std::endl;
	if (name && path)
		add((char *)name, new subResourceLoader((relativePath + (char *)path).c_str()));
}

void	ResourceManager::loadHitboxes(const std::string &path, xmlNodePtr node)
{
	std::vector<hitbox> hitboxes;

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
					hitboxes.push_back({std::stof((char *)centerX), std::stof((char *)centerY), std::stof((char *)width), std::stof((char *)height)});
			}
			else
				_log.warning << "unknown hitbox type '" << currentNode->name << "'" << std::endl;
		}
	}

	if (hitboxes.size() > 0)
		add("hitboxes", new hitboxesLoader(hitboxes));
}

void	ResourceManager::loadReference(const std::string &path, xmlNodePtr node)
{
	xmlChar *name = xmlGetProp(node, (const xmlChar *)"name");
	xmlChar *reference = xmlGetProp(node, (const xmlChar *)"reference");

	if (!name)
		_log.warning << "reference without name" << std::endl;
	if (!reference)
		_log.warning << "reference without reference" << std::endl;
	if (name && reference)
		add((char *)name, new referenceLoader((char *)reference));
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
	_log.info << "loading resource file '" << file << "', resources relative path: " << path << std::endl;
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
				else if (!xmlStrcmp(currentNode->name, (const xmlChar*)"reference"))
					loadReference(path, currentNode);
				else
					_log.warning << "unknown resource type '" << currentNode->name << "'" << std::endl;
			}
		}
	}

	xmlFreeDoc(doc);
}

ResourceManager::ILoader::ILoader(void)
{
}

ResourceManager::ILoader::~ILoader(void)
{
}

std::shared_ptr<IResource> &ResourceManager::ILoader::get(void)
{
	if (!_resource.get())
		this->load();
	return (_resource);
}

ResourceManager::fontLoader::fontLoader(const std::string &file, const std::string &texture) : _file(file), _texture(texture)
{
}

ResourceManager::fontLoader::~fontLoader(void)
{
}

void	ResourceManager::fontLoader::load(void)
{
	_resource = std::dynamic_pointer_cast<IResource>(std::shared_ptr<Font>(new Font(std::shared_ptr<FntLoader>(new FntLoader(_file)), std::shared_ptr<ITexture>(SManager::Get().getRenderer()->createTexture(_texture)))));
}

ResourceManager::textureLoader::textureLoader(const std::string &file) : _file(file)
{
}

ResourceManager::textureLoader::~textureLoader(void)
{
}

void	ResourceManager::textureLoader::load(void)
{
	_resource = std::dynamic_pointer_cast<IResource>(std::shared_ptr<ITexture>(SManager::Get().getRenderer()->createTexture(_file)));
}

ResourceManager::arrayTextureLoader::arrayTextureLoader(int width, int height, std::vector<std::string> textures, TextureFilter filter) : _width(width), _height(height), _textures(textures), _filter(filter)
{
}

ResourceManager::arrayTextureLoader::~arrayTextureLoader(void)
{
}

void	ResourceManager::arrayTextureLoader::load(void)
{
	_resource = std::dynamic_pointer_cast<IResource>(std::shared_ptr<IArrayTexture>(SManager::Get().getRenderer()->createArrayTexture(_width, _height, _textures, _filter)));
}

ResourceManager::soundLoader::soundLoader(const std::string &file) : _file(file)
{
}

ResourceManager::soundLoader::~soundLoader(void)
{
}

void	ResourceManager::soundLoader::load(void)
{
	_resource = std::dynamic_pointer_cast<IResource>(std::shared_ptr<ISound>(SManager::Get().getAudio()->createSound(_file)));
}

ResourceManager::subResourceLoader::subResourceLoader(const std::string &file) : _file(file)
{
	load();
}

ResourceManager::subResourceLoader::~subResourceLoader(void)
{
}

void	ResourceManager::subResourceLoader::load(void)
{
	_resource = std::dynamic_pointer_cast<IResource>(std::shared_ptr<ResourceManager>(new ResourceManager()));
	((ResourceManager *)_resource.get())->load(_file);
}

ResourceManager::hitboxesLoader::hitboxesLoader(const std::vector<hitbox> &hitboxes): _hitboxes(hitboxes)
{
}

ResourceManager::hitboxesLoader::~hitboxesLoader(void)
{
}

void	ResourceManager::hitboxesLoader::load(void)
{
	_resource = std::dynamic_pointer_cast<IResource>(std::shared_ptr<hitboxes>(new hitboxes(_hitboxes)));
}

ResourceManager::referenceLoader::referenceLoader(const std::string &reference) : _reference(reference)
{
}

ResourceManager::referenceLoader::~referenceLoader(void)
{
}

void	ResourceManager::referenceLoader::load(void)
{
	_resource = ResourceManager::Get().get<IResource>(_reference);
}
