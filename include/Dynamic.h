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

#include <map>
#include <string>
#include <typeinfo>

namespace	ExoEngine
{

class		IAttribute
{
	public:
		IAttribute(const std::string& name, const std::type_info* type);
		virtual ~IAttribute(void);

		const std::type_info*	type(void) const;
		const std::string&		name(void) const;

		virtual std::string	toString(void) const = 0;

		virtual int8_t		toInt8(void) const = 0;
		virtual int16_t		toInt16(void) const = 0;
		virtual int32_t		toInt32(void) const = 0;
		virtual int64_t		toInt64(void) const = 0;

		virtual uint8_t		toUInt8(void) const = 0;
		virtual uint16_t	toUInt16(void) const = 0;
		virtual uint32_t	toUInt32(void) const = 0;
		virtual uint64_t	toUInt64(void) const = 0;

		virtual float		toFloat(void) const = 0;
		virtual double		toDouble(void) const = 0;
	private:
		std::string				_name;
		const std::type_info*	_type;
};

template	<typename T>
class		Attribute : public IAttribute
{
	public:
		Attribute(const std::string& name, const T& src) : IAttribute(name, &typeid(T)), _data(src)
		{	}
		virtual ~Attribute(void)
		{	}

		virtual std::string	toString(void) const
		{
			if (typeid(T) == typeid(std::string))
				return ((std::string&)_data);
			else if (typeid(T) == typeid(int8_t))
				return (std::to_string((int8_t&)_data));
			else if (typeid(T) == typeid(int16_t))
				return (std::to_string((int16_t&)_data));
			else if (typeid(T) == typeid(int32_t))
				return (std::to_string((int32_t&)_data));
			else if (typeid(T) == typeid(int64_t))
				return (std::to_string((int64_t&)_data));
			else if (typeid(T) == typeid(uint8_t))
				return (std::to_string((uint8_t&)_data));
			else if (typeid(T) == typeid(uint16_t))
				return (std::to_string((uint16_t&)_data));
			else if (typeid(T) == typeid(uint32_t))
				return (std::to_string((uint32_t&)_data));
			else if (typeid(T) == typeid(uint64_t))
				return (std::to_string((uint64_t&)_data));
			else if (typeid(T) == typeid(float))
				return (std::to_string((float&)_data));
			else if (typeid(T) == typeid(double))
				return (std::to_string((double&)_data));
			return ("");
		}

		virtual int8_t		toInt8(void) const
		{
			return ((int8_t&)_data);
		}
		virtual int16_t		toInt16(void) const
		{
			return ((int16_t&)_data);
		}
		virtual int32_t		toInt32(void) const
		{
			return ((int32_t&)_data);
		}
		virtual int64_t		toInt64(void) const
		{
			return ((int64_t&)_data);
		}

		virtual uint8_t		toUInt8(void) const
		{
			return ((uint8_t&)_data);
		}
		virtual uint16_t	toUInt16(void) const
		{
			return ((uint16_t&)_data);
		}
		virtual uint32_t	toUInt32(void) const
		{
			return ((uint32_t&)_data);
		}
		virtual uint64_t	toUInt64(void) const
		{
			return ((uint64_t&)_data);
		}

		virtual float		toFloat(void) const
		{
			return ((float&)_data);
		}
		virtual double		toDouble(void) const
		{
			return ((double&)_data);
		}

		T&			value(void)
		{
			return ((T&)_data);
		}

		const T&	value(void) const
		{
			return ((T&)_data);
		}
	private:
		T	_data;
};

class	Dynamic : public IAttribute
{
	public:
		Dynamic(void);
		Dynamic(const std::string& name);
		Dynamic(const std::string& name, const std::type_info* type);
		virtual ~Dynamic(void);

		virtual std::string	toString(void) const;
		virtual int8_t		toInt8(void) const;
		virtual int16_t		toInt16(void) const;
		virtual int32_t		toInt32(void) const;
		virtual int64_t		toInt64(void) const;
		virtual uint8_t		toUInt8(void) const;
		virtual uint16_t	toUInt16(void) const;
		virtual uint32_t	toUInt32(void) const;
		virtual uint64_t	toUInt64(void) const;
		virtual float		toFloat(void) const;
		virtual double		toDouble(void) const;

		void				addAttribute(IAttribute* attribute);
		template			<typename T>
		void				addAttribute(const std::string& name, const T& attribute)
		{
			addAttribute(name, static_cast<IAttribute*>(new Attribute<T>(name, attribute)));
		}

		bool				hasAttribute(const std::string& name) const;
		size_t				getNbAttribute(void) const;
		IAttribute*			getAttribute(size_t index);
		const IAttribute*	getAttribute(size_t index) const;
		IAttribute*			getAttribute(const std::string& name);
		const IAttribute*	getAttribute(const std::string& name) const;
		template			<typename T>
		T&					getAttribute(const std::string& name)
		{
			return (static_cast<Attribute<T>*>(getAttribute(name))->value());
		}

		void				removeAttribute(const std::string& name);
		void				removeAttribute(const IAttribute* attribute);

		void				clearAttributes(void);
	private:
		std::map<std::string, IAttribute*>	_attributes;
};

}
