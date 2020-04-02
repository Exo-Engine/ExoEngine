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

#include "Runner.h"
#include "TaskQueue.h"
#include <chrono>
#include "Log.h"

using namespace	ExoEngine;

Runner::Runner(TaskQueue &queue) : _queue(queue), _thread(&Runner::loop, this), _ended(false)
{
	_thread.detach();
}

Runner::~Runner(void)
{
}

bool	Runner::ended(void)
{
	return (_ended);
}

void	Runner::loop(void)
{
	bool	handled;
	Task	task;

	while (1)
	{
		if (_queue.joining())
		{
			_ended = true;
			return ;
		}
		try
		{
			task = _queue.getTask();
			handled = true;
		}
		catch (const std::exception &)
		{
			handled = false;
		}
		if (handled)
		{
			task.launch();
			task.finish();
		}
		else
			std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(1));
	}
}
