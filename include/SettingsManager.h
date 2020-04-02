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

#include <libxml/parser.h>
#include <vector>
#include <map>

namespace	ExoEngine
{

class	Setting
{
	public:
		Setting(const std::string& name);
		virtual ~Setting(void);

		void				setName(const std::string& name);
		const std::string&	getName(void) const;

		void				addProperty(const std::string& name, const std::string& value);
		const std::string&	getProperty(const std::string& name);

		void				addChild(Setting* setting);
		size_t				getNbChilds(void) const;
		Setting*			getChild(size_t index);

		void				setValue(const std::string& value);
		const std::string&	getValue(void) const;
	private:
		std::string							_name;
		std::vector<Setting*>				_childs;
		std::map<std::string, std::string>	_properties;
		std::string							_value;
};

class	SettingsManager
{
	public:
		SettingsManager(void);
		virtual ~SettingsManager(void);

		void	load(const std::string& file);
		void	unload(void);
		void	save(const std::string& file);

		Setting*	get(const std::string& setting);

	private:

		void	load(xmlNodePtr node, Setting* parentSetting);

		std::map<std::string, Setting*>	_settings;
};

}
