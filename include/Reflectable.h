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

#include "Message.h"
#include "network/network.h"
#include "Log.h"

#include <string>

#define REFLECT_ATTRIBUTES_START()		\
	virtual void	initReflection(void)\
	{\

#define REFLECT_ATTRIBUTE(attr)		\
		if (std::is_base_of<IReflectable, decltype(attr)>::value == true)\
			_members.push_back((reflection::IReflectable*)&attr);\
		else\
			_members.push_back(new reflection::ReflectableType<decltype(attr)>(attr));

#define REFLECT_ATTRIBUTES_END()		\
	}

namespace	ExoEngine
{

namespace	reflection
{
	class		IReflectable
	{
		public:
			IReflectable(void);
			virtual ~IReflectable(void);

			void	read(Message& src);

			virtual void	write(Message& dst) = 0;
			virtual size_t	read(Message& src, size_t index) = 0;
		private:
	};

	template	<typename T>
	class		ReflectableType : public IReflectable
	{
		public:
			ReflectableType(T& data) : _data(data)
			{
			}
			virtual ~ReflectableType(void)
			{
			}

			virtual void	write(Message& dst)
			{
				if (typeid(T) == typeid(std::string))
				{
					dst.append(((std::string&)_data).length());
					dst.append(_data);
				}
				else
				{
					dst.append(network::endian(_data));
				}
			}
			virtual size_t	read(Message& src, size_t index)
			{
				if (typeid(T) == typeid(std::string))
				{
					std::string::size_type	size;

					memcpy((void*)&size, &src[index], sizeof(size));
					((std::string&)_data).resize(size);
					memcpy(&((std::string&)_data)[0], &src[index] + sizeof(size), size);
					return (index + sizeof(size) + size);
				}
				else
				{
					if (index + sizeof(T) > src.getSize())
						throw (std::invalid_argument("cannot read " + std::to_string(sizeof(T)) +
							" bytes, only " + std::to_string(src.getSize() - index) + " left"));
					memcpy((void*)&_data, &src[index], sizeof(T));
					ENDIAN(_data);
					return (index + sizeof(T));
				}
			}
		private:
			T&	_data;
	};

	class		ReflectableClass : public IReflectable
	{
		public:
			ReflectableClass(void);
			~ReflectableClass(void);


			virtual void	write(Message& dst);
			virtual size_t	read(Message& src, size_t index);
		protected:
			virtual void	initReflection(void) = 0;

			std::vector<IReflectable*>	_members;
	};
}

}
