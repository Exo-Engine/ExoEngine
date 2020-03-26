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

#include "Script.h"

#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/loader.h>
#include <glm/vec3.hpp>
#include <stdlib.h>

/*
**	script class and mono
*/

static MonoDomain	*domain = nullptr;

void	init_mono(const std::string &app, const std::string &version)
{
	domain = mono_jit_init_version(app.c_str(), version.c_str());

	if (!domain)
		throw (std::runtime_error("cannot init mono jit"));
}

void	exit_mono(void)
{
	if (domain)
		mono_jit_cleanup(domain);
}

/*
**	Script class
*/

Script::Script(const std::string &file, bool compiled) : Script(file, "", compiled)
{
}

Script::Script(const std::string &file, const std::string &mainClass, bool compiled) : _image(nullptr), _assembly(nullptr), _mainClass(mainClass)
{
	int	error;

	if (!domain)
		throw (std::runtime_error("mono must be initialized before using Script"));
	if (compiled)
	{
		_assembly = mono_domain_assembly_open(domain, file.c_str());
		if (!_assembly)
			throw (std::runtime_error("cannot load script assembly"));
	}
	else
	{
		error = system(("mcs -target:library " + file).c_str());
		if (error != EXIT_SUCCESS)
			throw (std::runtime_error("cannot compile script"));

		_assembly = mono_domain_assembly_open(domain, std::string(file, 0, file.length() - 2).append("dll").c_str());
		if (!_assembly)
			throw (std::runtime_error("cannot load script assembly"));
	}

	_image = mono_assembly_get_image(_assembly);
	if (!_image)
	{
		mono_assembly_close(_assembly);
		throw (std::runtime_error("cannot get image from script assembly"));
	}
}

Script::~Script(void)
{
	if (_assembly)
		mono_assembly_close(_assembly);
}

const std::string	&Script::getMainClass(void) const
{
	return (_mainClass);
}

Script::Class	*Script::get(const std::string &className)
{
	Script::Class	*klass;

	try
	{
		klass = _classes.at(className);
	}
	catch (const std::exception &e)
	{
		klass = new Class(*this, className);
		_classes[className] = klass;
	}
	return (klass);
}

Script::Class::Class(Script &script, const std::string &className) : _script(script)
{
	_class = mono_class_from_name(_script._image, "", className.c_str());
	if (!_class)
		throw (std::runtime_error("cannot get class " + className + " from script assembly"));

	MonoMethod	*method;
	void		*pointer = nullptr;

	while ((method = mono_class_get_methods(_class, &pointer)))
	{
		_methods[mono_method_get_name(method)] = method;
	}
}

Script::Class::~Class(void)
{
}

MonoObject	*Script::Class::call(const std::string &name, void *object, void **params) const
{
	try
	{
		return (mono_runtime_invoke(_methods.at(name), object, params, nullptr));
	}
	catch (const std::exception &e)
	{
		throw (std::runtime_error("cannot find method " + name));
	}
}

MonoObject	*Script::Class::newInstance(void) const
{
	if (_class)
		return (mono_object_new(domain, _class));
	throw (std::runtime_error("cannot create script instance without class"));
}
