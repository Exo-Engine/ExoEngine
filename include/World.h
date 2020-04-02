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

#include "Object.h"

#include <map>
#include <deque>
#include <glm/vec3.hpp>

namespace	ExoEngine
{

class	World : public std::recursive_mutex
{
	public:
		World(void);
		~World(void);

		void						clear(void);

		void						add(Object *object);
		void						removeObject(size_t id);
		void						removeObject(Object *object);
		Object						*getObject(size_t id);

		std::map<size_t, Object *>	&getObjects(void);

		void						addPlayer(size_t id, const std::string &name);
		void						removePlayer(size_t id);
		const std::string			&getPlayer(size_t id);

		void						setName(const std::string &name);
		void						setMusic(const std::string &music);

		void						setCameraType(int type);
		int						 getCameraType(void) const;

		void						setCameraPos(const glm::vec3 &pos);
		const glm::vec3			 &getCameraPos(void) const;

		const std::string	&getName(void) const;
		const std::string	&getMusic(void) const;
	private:
		std::map<size_t, std::string>	_playersMap;
		std::map<size_t, Object *>		_objectsMap;

		std::string					 _mapName;
		std::string					 _mapMusic;
		int							 _cameraType;
		glm::vec3						_cameraPos;
};

}
