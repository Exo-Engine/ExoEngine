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

#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>
#include <map>
#include <libxml/parser.h>

#include "Singleton.h"
#include "IResourceManager.h"
#include "IShader.h"
#include "Font.h"
#include "hitboxes.h"

#include "Log.h"

namespace	ExoEngine
{

class ResourceManager : public Singleton<ResourceManager>, public IResourceManager
{
	public:
		ResourceManager(void);
		virtual ~ResourceManager(void);

		virtual void load(const std::string &file);

		virtual const std::string	&getProperty(const std::string &name);

		virtual std::shared_ptr<IResource> get(const std::string& name);

		template <typename T>
		std::shared_ptr<T> get(const std::string& name)
		{
			auto res = _resources.find(name);
			if (res != _resources.end())
				return ((std::shared_ptr<T> &)(res->second->get()));

			return nullptr;
		}

	private:

		void	loadProperties(xmlNodePtr node);

		void	loadFont(const std::string &path, xmlNodePtr node);
		void	loadTexture(const std::string &path, xmlNodePtr node);
		void	loadArrayTexture(const std::string &path, xmlNodePtr node);
		void	loadSound(const std::string &path, xmlNodePtr node);
		void	loadSubResource(const std::string &path, xmlNodePtr node);
		void	loadHitboxes(const std::string &path, xmlNodePtr node);
		void	loadReference(const std::string &path, xmlNodePtr node);

		class	ILoader
		{
			public:
				ILoader(void);
				virtual ~ILoader(void);

				std::shared_ptr<IResource>	 &get(void);

			private:
				virtual void	load(void) = 0;

			protected:
				std::shared_ptr<IResource> _resource;
		};

		class	fontLoader : public ILoader
		{
			public:
				fontLoader(const std::string &file, const std::string &texture);
				~fontLoader(void);
			private:
				virtual void	load(void);

				std::string	 _file;
				std::string	 _texture;
		};

		class	textureLoader : public ILoader
		{
			public:
				textureLoader(const std::string &file);
				~textureLoader(void);
			private:
				virtual void	load(void);

				std::string	 _file;
		};

		class	arrayTextureLoader : public ILoader
		{
			public:
				arrayTextureLoader(int width, int height, std::vector<std::string> textures, ExoRenderer::TextureFilter filter);
				~arrayTextureLoader(void);
			private:
				virtual void	load(void);

				std::string				 _file;
				int						 _width;
				int						 _height;
				ExoRenderer::TextureFilter	_filter;
				std::vector<std::string>	_textures;
		};

		class	soundLoader : public ILoader
		{
			public:
				soundLoader(const std::string &file);
				~soundLoader(void);
			private:
				virtual void	load(void);

				std::string	 _file;
		};

		class	subResourceLoader : public ILoader
		{
			public:
				subResourceLoader(const std::string &file);
				~subResourceLoader(void);
			private:
				virtual void	load(void);

				std::string	 _file;
		};

		class	hitboxesLoader : public ILoader
		{
			public:
				hitboxesLoader(const std::vector<hitbox> &hitboxes);
				~hitboxesLoader(void);
			private:
				virtual void		load(void);
			
				std::vector<hitbox> _hitboxes;
		};

		class	referenceLoader : public ILoader
		{
			public:
				referenceLoader(const std::string &reference);
				~referenceLoader(void);
			private:
				virtual void	load(void);

				std::string	 _reference;
		};

		void	add(const std::string &name, ILoader *loader);

		friend class Singleton<ResourceManager>;
		std::unordered_map<std::string, ILoader *>	_resources;
		std::map<std::string, std::string>			_properties;
};

}
