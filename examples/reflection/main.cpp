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

#include "Reflectable.h"
#include <iostream>

using namespace	ExoEngine;
using namespace	reflection;

//	ClassA is a basic reflectable class, reflecting an integer and a string
class	ClassA : public ReflectableClass
{
	public:
		ClassA()
		{
			initReflection();
		}

		bool	operator==(const ClassA& b)
		{
			if (_a == b._a && _s == b._s)
				return (true);
			return (false);
		}

		int			_a;
		std::string	_s;

		//	we choose to reflect attributes a and s
		REFLECT_ATTRIBUTES_START()
		REFLECT_ATTRIBUTE(_a)
		REFLECT_ATTRIBUTE(_s)
		REFLECT_ATTRIBUTES_END()
};

//	ClassB is a class heriting reflection
class	ClassB : public ClassA
{
	public:
		ClassB()
		{
			initReflection();
		}

		bool	operator==(const ClassB& b)
		{
			if (_a == b._a && _s == b._s && _b == b._b)
				return (true);
			return (false);
		}

		//	we choose to reflect attribute b
		uint64_t	_b;
		REFLECT_ATTRIBUTES_START()
		REFLECT_ATTRIBUTE(_b)
		REFLECT_ATTRIBUTES_END()
};


int	main(void)
{
	Message	message;
	ClassA		testAInit;
	ClassB		testBInit;
	ClassA		testAResult;
	ClassB		testBResult;

	//	we initialize ClassA with some attributes
	testAInit._a = 42;
	testAInit._s = "TestA";
	//	write ClassA inside a buffer
	testAInit.write(message);
	//	read ClassA from the buffer
	testAResult.read(message, 0);
	std::cout << "testA:" << std::endl;
	std::cout << "\ta=" << testAResult._a << std::endl;
	std::cout << "\ts=" << testAResult._s << std::endl;

	if (testAInit == testAResult)
		std::cout << "reflection success for ClassA" << std::endl;
	else
		std::cout << "reflection failure for ClassA" << std::endl;

	message.resize(0);

	//	we initialize ClassB with some attributes
	testBInit._a = 24;
	testBInit._s = "testB";
	testBInit._b = 66;
	//	write ClassB inside a buffer
	testBInit.write(message);
	//	read ClassB from the buffer
	testBResult.read(message, 0);
	std::cout << "testB:" << std::endl;
	std::cout << "\ta=" << testBResult._a << std::endl;
	std::cout << "\ts=" << testBResult._s << std::endl;
	std::cout << "\tb=" << testBResult._b << std::endl;

	if (testBInit == testBResult)
		std::cout << "reflection success for ClassB" << std::endl;
	else
		std::cout << "reflection failure for ClassB" << std::endl;
	return (0);
}
