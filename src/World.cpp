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

#include "World.h"
#include "ResourceManager.h"

using namespace	ExoEngine;

World::World(void) : _mapName(""), _mapMusic(""), _cameraType(-1), _cameraPos(0, 0, 0)
{
}

World::~World(void)
{
	lock();
	clear();
	unlock();
}

void						World::clear(void)
{
	lock();
	for (auto object = _objectsMap.begin(); object != _objectsMap.end(); object++)
		delete object->second;
	_objectsMap.clear();
	unlock();
}

void						World::add(Object *object)
{
	lock();
	try
	{
		_objectsMap[object->getId()] = object;
	}
	catch (const std::exception &e)
	{
		_log.error << "cannot add object to world: " << e.what() << std::endl;
	}
	unlock();
}

void						World::removeObject(size_t id)
{
	removeObject(getObject(id));
}

void						World::removeObject(Object *object)
{
	lock();
	_log.debug << "unloading object " << object->getId() << std::endl;
	_objectsMap.erase(object->getId());
	delete object;
	unlock();
}

Object						*World::getObject(size_t id)
{
	Object	*object;

	lock();
	try
	{
		object = _objectsMap.at(id);
	}
	catch (const std::exception &)
	{
		object = nullptr;
	}
	unlock();
	return (object);
}

std::map<size_t, Object *>	&World::getObjects(void)
{
	return (_objectsMap);
}

void						World::addPlayer(size_t id, const std::string &name)
{
	lock();
	try
	{
		_playersMap[id] = name;
	}
	catch (const std::exception &e)
	{
		_log.error << "cannot add player to world: " << e.what() << std::endl;
	}
	unlock();
}

void						World::removePlayer(size_t id)
{
	lock();
	_playersMap.erase(_playersMap.find(id));
	unlock();
}

const std::string			&World::getPlayer(size_t id)
{
	return (_playersMap.at(id));
}

void						World::setName(const std::string &name)
{
	_mapName = name;
}

void						World::setMusic(const std::string &music)
{
	_mapMusic = music;
}

const std::string			&World::getName(void) const
{
	return _mapName;
}

const std::string			&World::getMusic(void) const
{
	return _mapMusic;
}

void						 World::setCameraType(int type)
{
	_cameraType = type;
}

int							World::getCameraType(void) const
{
	return _cameraType;
}

void						 World::setCameraPos(const glm::vec3 &pos)
{
	_cameraPos = pos;
}

const glm::vec3				&World::getCameraPos(void) const
{
	return _cameraPos;
}
