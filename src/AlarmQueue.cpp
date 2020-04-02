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

#include "AlarmQueue.h"

using namespace	ExoEngine;

AlarmQueue::AlarmQueue(TaskQueue &taskQueue) : _taskQueue(taskQueue)
{
}

AlarmQueue::~AlarmQueue(void)
{
	_alarms.clear();
}

void	AlarmQueue::add(const Alarm &alarm)
{
	try
	{
		_mutex.lock();
	}
	catch (const std::exception &)
	{
		return ;
	}
	for (auto list = _alarms.begin(); list != _alarms.end(); list++)
		if (alarm <= *list)
		{
			_alarms.insert(list--, alarm);
			_mutex.unlock();
			return ;
		}
	_alarms.push_back(alarm);
	_mutex.unlock();
}

void	AlarmQueue::manage(void)
{
	auto now = std::chrono::high_resolution_clock::now();

	try
	{
		_mutex.lock();
	}
	catch (const std::exception &)
	{
		return ;
	}
	for (auto list = _alarms.begin(); list != _alarms.end(); list++)
		if ((*list).elapsed(now))
		{
			_taskQueue.add((*list).getTask());
			_alarms.erase(list);
			if (_alarms.size() <= 1)
			{
				_mutex.unlock();
				return ;
			}
		}
		else
		{
			_mutex.unlock();
			return ;
		}
	_mutex.unlock();
}
