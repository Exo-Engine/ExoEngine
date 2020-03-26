/*
 *  MIT License
 *  
 *  Copyright (c) 2020 Gaëtan Dezeiraud and Ribault Paul
 *  
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *  
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *  
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */

#pragma once

#include <mono/jit/jit.h>

#include <string>
#include <map>

#include "IResource.h"

class	Script : public IResource
{
	public:
		Script(const std::string &file, bool compiled);
		Script(const std::string &file, const std::string &mainClass, bool compiled);
		~Script(void);

		const std::string	&getMainClass(void) const;

		class	Class
		{
			public:
				Class(Script &script, const std::string &className);
				~Class(void);

				MonoObject	*call(const std::string &method, void *object, void **params) const;
				MonoObject	*newInstance(void) const;
			private:
				Script								&_script;
				MonoClass							*_class;
				std::map<std::string, MonoMethod *>	_methods;
		};

		Class	*get(const std::string &className);

	protected:
		MonoImage						*_image;
	private:
		MonoAssembly					*_assembly;
		std::map<std::string, Class *>	_classes;
		std::string						_mainClass;
};

void	init_mono(const std::string &app, const std::string &version);
void	exit_mono(void);
