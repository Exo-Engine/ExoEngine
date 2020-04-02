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

#include "Local.h"
#include "Log.h"
#include <iostream>
#include <libxml/parser.h>

using namespace	ExoEngine;

Local::Local(void)
{	}

Local::~Local(void)
{	}

void Local::loadLocal(const std::string &lang)
{
	_words.clear();
	_currentLang = lang;

	// Load XML
	xmlDocPtr doc;
	xmlNodePtr root;

	doc = xmlParseFile(std::string("resources/langs/" + _currentLang + ".xml").c_str());
	if (doc == NULL)
	{
		_currentLang = "en"; // By Default
		doc = xmlParseFile(std::string("resources/langs/" + _currentLang + ".xml").c_str());
	}

	root = xmlDocGetRootElement(doc);
	if (!root)
	{
		xmlFreeDoc(doc);
		throw (std::invalid_argument("cannot get root node on 'resources/langs/" + _currentLang + ".xml'"));
	}

	// Read XML
	if (root->type == XML_ELEMENT_NODE && xmlStrcmp(root->name, (const xmlChar*)"resources") == 0)
	{
		for (auto currentNode = root->children; currentNode; currentNode = currentNode->next)
		{
			if (currentNode->type == XML_ELEMENT_NODE)
			{
				if (!xmlStrcmp(currentNode->name, (const xmlChar*)"string"))
				{
					_words.insert(std::pair<std::string,std::string>(
						(char*)xmlGetProp(currentNode,(const xmlChar *)"name"),
						(char*)xmlNodeGetContent(currentNode)
					));
				}
				else
					_log.warning << "unknown language type '" << currentNode->name << "'" << std::endl;
			}
		}
	}

	xmlFreeDoc(doc);
}

const std::string &Local::get(const std::string &id)
{
	auto it = _words.find(id);
	if (it != _words.end())
		return it->second;
	else
		return id;
}
