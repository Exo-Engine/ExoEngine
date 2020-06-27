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

#include "Dynamic.h"
#include <stdexcept>

using namespace	ExoEngine;

IAttribute::IAttribute(const std::string& name, const std::type_info* type) : _name(name), _type(type)
{
}

IAttribute::~IAttribute(void)
{
}

const std::type_info*	IAttribute::type(void) const
{
	return (_type);
}

const std::string&		IAttribute::name(void) const
{
	return (_name);
}

Dynamic::Dynamic(void) : Dynamic("")
{
}

Dynamic::Dynamic(const std::string& name) : Dynamic(name, &typeid(Dynamic))
{
}

Dynamic::Dynamic(const std::string& name, const std::type_info* type) : IAttribute(name, type)
{
}

Dynamic::~Dynamic(void)
{
	clearAttributes();
}

std::string	Dynamic::toString(void) const
{
	return ("");
}

int8_t		Dynamic::toInt8(void) const
{
	return (0);
}

int16_t		Dynamic::toInt16(void) const
{
	return (0);
}

int32_t		Dynamic::toInt32(void) const
{
	return (0);
}

int64_t		Dynamic::toInt64(void) const
{
	return (0);
}

uint8_t		Dynamic::toUInt8(void) const
{
	return (0);
}

uint16_t	Dynamic::toUInt16(void) const
{
	return (0);
}

uint32_t	Dynamic::toUInt32(void) const
{
	return (0);
}

uint64_t	Dynamic::toUInt64(void) const
{
	return (0);
}

float		Dynamic::toFloat(void) const
{
	return (0);
}

double		Dynamic::toDouble(void) const
{
	return (0);
}

void				Dynamic::addAttribute(IAttribute* attribute)
{
	if (_attributes.find(attribute->name()) != _attributes.end())
		throw(std::invalid_argument("attribute '" + attribute->name() + "'already present"));
	_attributes[attribute->name()] = attribute;
}

bool				Dynamic::hasAttribute(const std::string& name) const
{
	std::map<std::string, IAttribute*>::const_iterator	iterator;

	iterator = _attributes.find(name);
	if (iterator == _attributes.cend())
		return (false);
	return (true);
}

size_t				Dynamic::getNbAttribute(void) const
{
	return (_attributes.size());
}

IAttribute*			Dynamic::getAttribute(size_t index)
{
	std::map<std::string, IAttribute*>::iterator	iterator;
	std::map<std::string, IAttribute*>::iterator	end;

	iterator = _attributes.begin();
	end = _attributes.end();
	for (size_t i = 0; iterator != end && i < index; i++)
		iterator++;
	if (iterator == end)
		return (nullptr);
	return (iterator->second);
}

const IAttribute*	Dynamic::getAttribute(size_t index) const
{
	std::map<std::string, IAttribute*>::const_iterator	iterator;
	std::map<std::string, IAttribute*>::const_iterator	end;

	iterator = _attributes.cbegin();
	end = _attributes.cend();
	for (size_t i = 0; iterator != end && i < index; i++)
		iterator++;
	if (iterator == end)
		return (nullptr);
	return (iterator->second);
}

IAttribute*			Dynamic::getAttribute(const std::string& name)
{
	std::map<std::string, IAttribute*>::iterator	iterator;

	iterator = _attributes.find(name);
	if (iterator == _attributes.end())
		return (nullptr);
	return (iterator->second);
}

const IAttribute*	Dynamic::getAttribute(const std::string& name) const
{
	std::map<std::string, IAttribute*>::const_iterator	iterator;

	iterator = _attributes.find(name);
	if (iterator == _attributes.cend())
		return (nullptr);
	return (iterator->second);
}

void				Dynamic::removeAttribute(const std::string& name)
{
	std::map<std::string, IAttribute*>::iterator	iterator;

	iterator = _attributes.find(name);
	if (iterator == _attributes.end())
		return ;
	delete iterator->second;
	_attributes.erase(iterator);
}

void				Dynamic::removeAttribute(const IAttribute* attribute)
{
	removeAttribute(attribute->name());
}

void				Dynamic::clearAttributes(void)
{
	std::map<std::string, IAttribute*>::iterator	iterator;
	std::map<std::string, IAttribute*>::iterator	end;

	end = _attributes.end();
	for (iterator = _attributes.begin(); iterator != end; iterator++)
	{
		delete iterator->second;
	}
	_attributes.clear();
}
