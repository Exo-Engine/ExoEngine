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

#include "Message.h"

using namespace	ExoEngine;

Message::Message(void)
{
}

Message::Message(const Message &src) : Message(src.getPtr(), src.getSize())
{
}

Message::Message(const std::string &src) : Message((void *)&src[0], src.length())
{
}

Message::Message(const void *ptr, size_t size)
{
	for (size_t i = 0; i < size; i++)
		_message.push_back(*(uint8_t *)((size_t)ptr + i));
}

Message::Message(size_t size) : _message(size)
{
}

Message::~Message(void)
{
}

Message	&Message::append(const Message &src)
{
	return (this->append(src.getPtr(), src.getSize()));
}

Message	&Message::append(const std::string &src)
{
	return (this->append((void *)&src[0], src.length()));
}

Message	&Message::append(const void *ptr, size_t size)
{
	for (size_t i = 0; i < size; i++)
		_message.push_back(*(uint8_t *)((size_t)ptr + i));
	return (*this);
}

Message	&Message::operator=(const Message &src)
{
	_message.resize(src.getSize());
	for (size_t i = 0; i < _message.size(); i++)
		_message[i] = const_cast<Message &>(src)[i];
	return (*this);
}

Message	&Message::operator=(const std::string &src)
{
	_message.resize(src.length());
	for (size_t i = 0; i < _message.size(); i++)
		_message[i] = (uint8_t)src[i];
	return (*this);
}

Message	&Message::operator+=(const Message &src)
{
	for (size_t i = 0; i < src.getSize(); i++)
		_message.push_back(const_cast<Message &>(src)[i]);
	return (*this);
}

Message	&Message::operator+=(const std::string &src)
{
	for (size_t i = 0; i < src.length(); i++)
		_message.push_back((uint8_t)src[i]);
	return (*this);
}

Message	Message::operator+(const Message &src) const
{
	Message	msg(*this);

	msg += src;
	return (msg);
}

Message	Message::operator+(const std::string &src) const
{
	Message	msg(*this);

	msg += src;
	return (msg);
}

uint8_t	&Message::operator[](size_t idx)
{
	return (_message[idx]);
}

void	Message::resize(size_t size)
{
	_message.resize(size);
}

void	Message::clear(void)
{
	_message.clear();
}

const void	*Message::getPtr(void) const
{
	if (_message.size())
		return ((void *)&_message[0]);
	return (nullptr);
}

size_t		Message::getSize(void) const
{
	return (_message.size());
}

std::string	Message::to_string(void) const
{
	std::string	str;

	str.append((const char *)getPtr(), getSize());
	return (str);
}

std::ostream	&operator<<(std::ostream &out, const Message &msg)
{
	return (out << msg.to_string());
}
