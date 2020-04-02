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

#include "Log.h"

using namespace	ExoEngine;

Log	ExoEngine::_log("log");

Log::Log(const std::string &file) : error(*this, "error", 196, _start), warning(*this, "warning", 208, _start), info(*this, "info", 190, _start), debug(*this, "debug", 46, _start), meeseeks(*this, "look at meeeee", 39, _start)
{
	try
	{
		_file.open(file.c_str(), std::ofstream::out | std::ofstream::app);
		_file << "___ Log start ___" << std::endl;
#ifdef LOG_TERM
		std::cout << "___ Log start ___" << std::endl;
#endif
	}
	catch (const std::exception &)
	{
		throw ;
	}
	_start = true;
}

Log::~Log()
{
	_file << "___ Log end ___" << std::endl;
#ifdef LOG_TERM
	std::cout << "___ Log end ___" << std::endl;
#endif
	_file.close();
}

Log		&Log::operator<<(std::ostream& (*pf)(std::ostream&))
{
	if (!pf)
		throw (std::exception());
	_mutex.lock();
	try
	{
#ifdef LOG_TERM
		pf(std::cout);
#endif
		pf(_file);
	}
	catch (const std::exception &)
	{
		_mutex.unlock();
		throw ;
	}
	_start = true;
	_mutex.unlock();
	return (*this);
}

Log::LogLeveled::LogLeveled(Log &log, const std::string &name, const uint8_t &color, const bool &start) : _log(log), _name(name), _color(color), _start(start), _enabled(true)
{
}

Log::LogLeveled::~LogLeveled(void)
{
}

Log::LogLeveled		&Log::LogLeveled::operator<<(std::ostream& (*pf)(std::ostream&))
{
	if (_enabled)
		_log << pf;
	return (*this);
}

void	Log::LogLeveled::disable(void)
{
	_enabled = false;
}

void	Log::LogLeveled::enable(void)
{
	_enabled = true;
}
