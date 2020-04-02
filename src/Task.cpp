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

#include "Task.h"
#include <string.h>

using namespace	ExoEngine;

Task::Task(void) : _function(nullptr), _finishCallback(nullptr), _cancelCallback(nullptr)
{
}

Task::Task(const Task &src)
{
	memcpy(this, &src, sizeof(Task));
}

Task::Task(void (*function)(void), void (*finishCallback)(void), void (*cancelCallback)(void)) : _function(function), _finishCallback(finishCallback), _cancelCallback(cancelCallback)
{
}

Task::~Task(void)
{
}

void	Task::launch(void) const
{
	if (_function)
		_function();
}

void	Task::finish(void) const
{
	if (_finishCallback)
		_finishCallback();
}

void	Task::cancel(void) const
{
	if (_cancelCallback)
		_cancelCallback();
}
