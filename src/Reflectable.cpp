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

#include "Reflectable.h"

using namespace	ExoEngine;

reflection::IReflectable::IReflectable(void)
{
}

reflection::IReflectable::~IReflectable(void)
{
}

void	reflection::IReflectable::read(Message& src)
{
	read(src, 0);
}

reflection::ReflectableClass::ReflectableClass(void)
{
}
reflection::ReflectableClass::~ReflectableClass(void)
{
}

void	reflection::ReflectableClass::write(Message& dst)
{
	for (auto member = _members.begin(); member != _members.end(); member++)
	{
		(*member)->write(dst);
	}
}

size_t	reflection::ReflectableClass::read(Message& src, size_t index)
{
	for (auto member = _members.begin(); member != _members.end(); member++)
	{
		try
		{
			index = (*member)->read(src, index);
		}
		catch (const std::exception &)
		{
			throw ;
		}
	}
	return (index);
}
