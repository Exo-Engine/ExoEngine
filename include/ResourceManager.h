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

#include "IShader.h"
#include "Font.h"
#include "hitboxes.h"
#include "IRenderer.h"
#include "IAudio.h"
#include "IResource.h"

#include "Log.h"

namespace	ExoEngine
{

class ResourceManager : public IResource
{
	public:
		ResourceManager(ExoRenderer::IRenderer* renderer, ExoAudio::IAudio* audio);
		virtual ~ResourceManager(void);

		ExoRenderer::IRenderer*	getRenderer(void) const;

		void load(const std::string &file);

		const std::string	&getProperty(const std::string &name);

		const std::shared_ptr<IResource>& get(const std::string& name);

		template <typename T>
		std::shared_ptr<T> get(const std::string& name)
		{
			auto res = _resources.find(name);
			if (res != _resources.end())
				return (std::dynamic_pointer_cast<T>(res->second));

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

		template	<typename T>
		void	add(const std::string &name, const std::shared_ptr<T> &resource)
		{
			_resources[name] = std::dynamic_pointer_cast<IResource>(resource);
		}

		ExoRenderer::IRenderer*								_renderer;
		ExoAudio::IAudio*									_audio;
		std::map<std::string, std::shared_ptr<IResource>>	_resources;
		std::map<std::string, std::string>					_properties;
};

}
