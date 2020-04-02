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

#include "TaskQueue.h"

using namespace	ExoEngine;

TaskQueue::TaskQueue(uint8_t nThreads) : _n(nThreads), _joining(false)
{
	_mutex.lock();
	for (uint8_t i = 0; i < _n; i++)
		_runners.push_back(new Runner(*this));
	_mutex.unlock();
}

TaskQueue::~TaskQueue(void)
{
	static const std::chrono::high_resolution_clock::duration	timeout = std::chrono::milliseconds(100);
	std::chrono::high_resolution_clock::time_point				start, end;

	_joining = true;
	_mutex.lock();
	_tasks.clear();
	for (uint8_t i = 0; i < _n; i++)
	{
		start = std::chrono::high_resolution_clock::now();
		end = start;
		while (!_runners[i]->ended() && end - start < timeout)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			end = std::chrono::high_resolution_clock::now();
		}
		delete _runners[i];
	}
}

void	TaskQueue::add(const Task &task)
{
	try
	{
		_mutex.lock();
	}
	catch (const std::exception &)
	{
		throw ;
	}
	_tasks.push(task);
	_mutex.unlock();
}

Task	TaskQueue::getTask(void)
{
	Task	task;

	try
	{
		if (!_mutex.try_lock())
			throw (std::runtime_error("cannot lock mutex"));
	}
	catch (const std::exception &)
	{
		throw ;
	}
	if (_tasks.isEmpty())
	{
		_mutex.unlock();
		throw (std::runtime_error("tasks queue empty"));
	}
	task = _tasks.pop();
	_mutex.unlock();
	return (task);
}

bool	TaskQueue::joining(void) const
{
	return (_joining);
}
