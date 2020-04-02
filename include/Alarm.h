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

#include "Task.h"
#include <ctime>
#include <chrono>

namespace	ExoEngine
{

class	Alarm
{
	public:
		Alarm(const Task &task, const std::chrono::time_point<std::chrono::high_resolution_clock> &time);
		Alarm(const Task &task, const std::chrono::high_resolution_clock::duration &time);
		~Alarm(void);

		const Task	&getTask(void) const;

		bool	elapsed(void) const;
		bool	elapsed(const std::chrono::time_point<std::chrono::high_resolution_clock> &now) const;

		bool	operator<(const Alarm &b) const;
		bool	operator>(const Alarm &b) const;
		bool	operator<=(const Alarm &b) const;
		bool	operator>=(const Alarm &b) const;
		bool	operator==(const Alarm &b) const;
		bool	operator!=(const Alarm &b) const;

	protected:
		std::chrono::time_point<std::chrono::high_resolution_clock>	_time;

	private:
		Task														_task;
};

}
