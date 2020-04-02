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

#include "Dynamic.h"
#include <iostream>
#include <vector>

using namespace	ExoEngine;

//	ClassA is a basic reflectable class, reflecting an integer and a string
class	ClassA : public Dynamic
{
	public:
		ClassA(void) {}
		~ClassA(void) {}
};

int	main(void)
{
	ClassA		testA;
	IAttribute*	attribute;

	testA.addAttribute("exampleInt", 42);
	testA.addAttribute("exampleLong", 2147483648);
	testA.addAttribute("exampleString", std::string("hello :D"));
	testA.addAttribute("exampleFloat", 42.42);
	testA.addAttribute("exampleVector", std::vector<int>({10, 11, 12}));
	for (size_t i = 0; i < testA.getNbAttribute(); i++)
	{
		attribute = testA.getAttribute(i);
		std::cout << "[" << attribute->name() << "]=" << attribute->toString() << std::endl;
		if (attribute->type() == &typeid(std::vector<int>))
		{
			std::vector<int> v = dynamic_cast<Attribute<std::vector<int>>*>(attribute)->value();
			for (auto value : v)
			{
				std::cout << " - " << value << std::endl;
			}
		}
	}
	return (0);
}
