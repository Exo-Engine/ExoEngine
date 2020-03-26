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

#include "Reflectable.h"

#include <stdexcept>

#define REFLECT_MEMBERS_START()		\
	virtual void	initReflection(void)\
	{\

#define REFLECT_MEMBER(attr)		\
		if (std::is_base_of<IReflectable, decltype(attr)>::value == true)\
			_members.push_back((reflection::IReflectable*)&attr);\
		else\
			_members.push_back(new reflection::ReflectableType<decltype(attr)>(attr));

#define REFLECT_MEMBERS_END()		\
	}

namespace	ExoEngine
{

namespace	reflection
{
	class	String : public ReflectableClass, public std::string
	{
		public:
			String(void)
			{
				initReflection();
			}
			String(const String &src)
			{
				initReflection();
				((std::string &)*this) = (const std::string &)src;
			}
			String(const std::string &src) : std::string(src)
			{
				initReflection();
			}
			virtual void	initReflection(void)
			{
			}
			virtual void	write(Message& dst)
			{
				dst.append(network::endian(length()));
				dst.append((void*)&(*this)[0], length());
			}
			virtual size_t	read(Message& src, size_t index)
			{
				size_type	size;
				size_type	i;

				//	get size
				if (index + sizeof(size_type) > src.getSize())
					throw (std::invalid_argument("cannot read " + std::to_string(sizeof(size_type)) +
						" bytes, only " + std::to_string(src.getSize() - index) + " left"));
				size = network::endian(*(size_type *)&src[index]);
				index += sizeof(size_type);

				//	get string
				if (index + size > src.getSize())
					throw (std::invalid_argument("cannot read " + std::to_string(size) +
						" bytes, only " + std::to_string(src.getSize() - index) + " left"));
				resize(size);
				for (i = 0; i < size; i++)
					(*this)[i] = src[index + i];
				index += size;
				return (index);
			}
			String		&operator=(const std::string &src)
			{
				return ((String &)(((std::string &)*this) = src));
			}
	};
}

}
