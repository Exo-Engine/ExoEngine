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

#include "Alarm.h"

using namespace	ExoEngine;

Alarm::Alarm(const Task &task, const std::chrono::time_point<std::chrono::high_resolution_clock> &time) : _time(time), _task(task)
{
}

Alarm::Alarm(const Task &task, const std::chrono::high_resolution_clock::duration &time) : _task(task)
{
	_time = std::chrono::high_resolution_clock::now() + time;
}

Alarm::~Alarm(void)
{
}

const Task	&Alarm::getTask(void) const
{
	return (_task);
}

bool	Alarm::elapsed(void) const
{
	return (std::chrono::high_resolution_clock::now() >= _time);
}

bool	Alarm::elapsed(const std::chrono::time_point<std::chrono::high_resolution_clock> &now) const
{
	return (now >= _time);
}

bool	Alarm::operator<(const Alarm &b) const
{
	return (_time < b._time);
}

bool	Alarm::operator>(const Alarm &b) const
{
	return (_time > b._time);
}

bool	Alarm::operator<=(const Alarm &b) const
{
	return (_time <= b._time);
}

bool	Alarm::operator>=(const Alarm &b) const
{
	return (_time >= b._time);
}

bool	Alarm::operator==(const Alarm &b) const
{
	return (_time == b._time);
}

bool	Alarm::operator!=(const Alarm &b) const
{
	return (_time != b._time);
}
