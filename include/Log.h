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

#include <string>
#include <fstream>
#include <mutex>
#include <time.h>

#define LOG_TERM
#if (defined __linux__ || defined __APPLE__)
# define LOG_COLORS
#endif

#ifdef LOG_TERM
# include <iostream>
#endif

#ifndef __linux__
# pragma warning(disable : 4996)
#endif

namespace	ExoEngine
{

class Log
{
	public:

		Log(const std::string &file);
		~Log();

		Log		&operator<<(std::ostream& (*pf)(std::ostream&));

		template	<typename T>
		Log		&operator<<(const T &s)
		{
			_mutex.lock();
			if (_start == true)
			{
				std::string	prefix;
				time_t		t = time(0);

				prefix.resize(20);
				prefix.resize(strftime(const_cast<char *>(prefix.c_str()), prefix.length(), "%G%m%d_%H%M%S", localtime(&t)));
				try
				{
#ifdef LOG_TERM
					std::cout << "[" << prefix << "] " << s;
#endif
					_file << "[" << prefix << "] " << s;
				}
				catch (const std::exception &)
				{
					_mutex.unlock();
					throw ;
				}
				_start = false;
			}
			else
			{
				try
				{
#ifdef LOG_TERM
					std::cout << s;
#endif
					_file << s;
				}
				catch (const std::exception &)
				{
					_mutex.unlock();
					throw ;
				}
			}
			_mutex.unlock();
			return (*this);
		}

	private:

		class	LogLeveled
		{
			public:
				LogLeveled(Log &log, const std::string &name, const uint8_t &color, const bool &start);
				~LogLeveled(void);

				LogLeveled		&operator<<(std::ostream& (*pf)(std::ostream&));

				template		<typename T>
				LogLeveled		&operator<<(const T &s)
				{
					if (_enabled)
					{
						if (_start)
#ifdef LOG_COLORS
						#if (defined __APPLE__)
							_log <<
								(_name == "error" ? "☠️"
								 : (_name == "warning") ? "⚠️"
								 : (_name == "info" ? "ℹ️"
									: (_name == "debug") ? "🐞"
									: (_name == "look at meeeee") ? "👋" : ""))
								<< "[" << _name << "] " << s;
						#else
							_log << "[\e[38;5;" << (int)_color << "m" << _name << "\e[0m" << "] " << s;
						#endif
#else
							_log << "[" << _name << "] " << s;
#endif
						else
							_log << s;
					}
					return (*this);
				}

				void	disable(void);
				void	enable(void);

			private:
				Log&		_log;
				std::string	_name;
				uint8_t		_color;
				const bool&	_start;
				bool		_enabled;
		};

	public:

		LogLeveled		error;
		LogLeveled		warning;
		LogLeveled		info;
		LogLeveled		debug;
		LogLeveled		meeseeks;

	private:

		std::ofstream	_file;
		bool			_start;
		std::mutex		_mutex;
};

extern Log	_log;

}
