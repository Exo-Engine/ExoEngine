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

#if (defined _WIN32 || defined _WIN64)
# include <Windows.h>
#endif

#ifndef _WIN32
# include <dlfcn.h>
#endif

#include <string>
#include <stdlib.h>

namespace	ExoEngine
{

template <class T>
class LibLoader
{
public:
	typedef T* (*LIB)();

	LibLoader(const std::string &path)
	{
		// Load
#ifdef _WIN32
		_library = LoadLibrary(TEXT(std::string(path + ".dll").c_str()));
		if (!_library) {
			std::cout << "could not load the dynamic library" << std::endl;
			exit(EXIT_FAILURE);
		}
#else

#ifdef __linux__
		_library = dlopen(std::string("./" + path + ".so").c_str(), RTLD_LAZY);
#else
		_library = dlopen(std::string("lib" + path + ".dylib").c_str(), RTLD_LAZY);
#endif

		if (!_library)
			throw (std::invalid_argument(std::string("could not load the dynamic library! ").append(dlerror())));
#endif

		// Use
#ifdef _WIN32
		FARPROC lib = (FARPROC)GetProcAddress(_library, "StartPlugin");
		if (!lib)
		{
			FreeLibrary(_library);
			throw (std::invalid_argument("could not locate the start function!"));
		}
#else
		LIB lib = reinterpret_cast<LIB>(dlsym(_library, "StartPlugin"));
		if (!lib)
		{
			dlclose(_library);
			throw (std::invalid_argument(std::string("could not locate the start function! ").append(dlerror())));
		}
#endif

		_pPointer = (T *)lib();
	}

	~LibLoader(void)
	{
		if (_pPointer)
			delete _pPointer;
#ifdef _WIN32
		if (_library)
			FreeLibrary(_library);
#else
		if (_library)
			dlclose(_library);
#endif
	}

	// Getters
	T* getPlugin(void)
	{
		return _pPointer;
	}

private:
	T* _pPointer;

#ifdef _WIN32
	HMODULE _library;
#else
	void* _library;
#endif
};

}
