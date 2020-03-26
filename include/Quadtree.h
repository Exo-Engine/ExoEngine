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

#include <glm/vec2.hpp>
#include "exoway.h"
#include <iostream>
#include <vector>
#include "Log.h"

template	<typename T>
class	Quadtree
{
	public:
		enum	type
		{
			TYPE_NODE,
			TYPE_LEAF
		};
		Quadtree(const glm::vec2 &pos, size_t size, const T &leaf) : _type(TYPE_LEAF), _parent(NULL), _leaf(leaf), _pos(pos), _size(size)
		{
		}

		~Quadtree(void)
		{
			if (_type == TYPE_NODE)
				for (size_t i = 0; i < 4; i++)
					delete _childs[i];
		}

		Quadtree<T>	*operator[](size_t index)
		{
			if (_type == TYPE_LEAF)
				throw (exoway::BasicException("Quadtree isn't a node but a leaf"));
			if (index >= 4)
				throw (exoway::BasicException("index greater or equal than 4"));
			return (_childs[index]);
		}

		const T		&at(const glm::vec2 &pos) const
		{
			if (pos.x >= _pos.x && pos.x < _pos.x + _size &&
				pos.y >= _pos.y && pos.y < _pos.y + _size)
				return (privateAt(pos));
			else
				throw (exoway::BasicException(std::string("position (").append(std::to_string(pos.x)).append(";").append(std::to_string(pos.y)).append(") out of quadtree")));
		}

		void		set(const glm::vec2 &pos, const T &value)
		{
			if (pos.x >= _pos.x && pos.x < _pos.x + _size &&
				pos.y >= _pos.y && pos.y < _pos.y + _size)
				return (privateSet(pos, value));
			else
				throw (exoway::BasicException(std::string("position (").append(std::to_string(pos.x)).append(";").append(std::to_string(pos.y)).append(") out of quadtree")));
		}

		void		debug(void)
		{
			privateDebug(0);
		}

		enum type	getType(void) const
		{
			return (_type);
		}

		T			&getLeaf(void)
		{
			return (_leaf);
		}

		Quadtree<T>	*getParent(void)
		{
			return (_parent);
		}

		size_t	getSize(void)
		{
			return (_size);
		}

		const glm::vec2	&getPos(void) const
		{
			return (_pos);
		}

		class	Iterator
		{
			public:
				Iterator(Quadtree<T> *prev, Quadtree<T> *current) : _prev(prev), _current(current)
				{
				}
				~Iterator(void)
				{
				}

				Iterator	&operator++(int)
				{
					if (_current)
					{
						if (_current->getType() == TYPE_NODE)
						{
							if (_prev == _current->getParent())
							{
								_prev = _current;
								_current = (*_current)[0];
							}
							else if (_prev == (*_current)[0])
							{
								_prev = _current;
								_current = (*_current)[1];
							}
							else if (_prev == (*_current)[1])
							{
								_prev = _current;
								_current = (*_current)[2];
							}
							else if (_prev == (*_current)[2])
							{
								_prev = _current;
								_current = (*_current)[3];
							}
							else
							{
								_prev = _current;
								_current = _current->getParent();
								return ((*this)++);
							}
						}
						else
						{
							_prev = _current;
							_current = _current->getParent();
							return ((*this)++);
						}
					}
					return (*this);
				}

				bool	operator==(const Iterator &b) const
				{
					if (_prev == b._prev &&
						_current == b._current)
						return (true);
					return (false);
				}

				bool	operator!=(const Iterator &b) const
				{
					if (_prev != b._prev ||
						_current != b._current)
						return (true);
					return (false);
				}

				Quadtree<T>	*operator*(void)
				{
					return (_current);
				}
			protected:
				Quadtree<T>				*_prev;
				Quadtree<T>				*_current;
		};

		Iterator	begin(void)
		{
			return (Iterator(NULL, this));
		}

		Iterator	end(void)
		{
			return (Iterator(this, NULL));
		}
	protected:
		Quadtree(const glm::vec2 &pos, size_t size, const T &leaf, Quadtree *parent) : _type(TYPE_LEAF), _parent(parent), _leaf(leaf), _pos(pos), _size(size)
		{
		}
	private:
		enum
		{
			CHILD_Y_MIN = 2,
			CHILD_Y_MAX = 0,
			CHILD_X_MIN = 0,
			CHILD_X_MAX = 1
		};

		const T		&privateAt(const glm::vec2 &pos) const
		{
			size_t	index = 0;

			if (_type == TYPE_NODE)
			{
				if (pos.x < _pos.x + _size / 2)
					index += CHILD_X_MIN;
				else
					index += CHILD_X_MAX;
				if (pos.y < _pos.y + _size / 2)
					index += CHILD_Y_MIN;
				else
					index += CHILD_Y_MAX;
				if (_childs[index])
					return (_childs[index]->privateAt(pos));
				else
					throw (exoway::BasicException("node with unassigned child"));
			}
			else if (_type == TYPE_LEAF)
				return (_leaf);
			else
				throw (exoway::BasicException("unknown quadtree type"));
		}

		void		privateSet(const glm::vec2 &pos, const T &value)
		{
			size_t	index = 0;
			bool	simplify;

			if (_type == TYPE_NODE)
			{
				if (pos.x < _pos.x + _size / 2)
					index += CHILD_X_MIN;
				else
					index += CHILD_X_MAX;
				if (pos.y < _pos.y + _size / 2)
					index += CHILD_Y_MIN;
				else
					index += CHILD_Y_MAX;
				if (_childs[index])
				{
					_childs[index]->privateSet(pos, value);
					simplify = true;
					for (size_t i = 0; i < 4 && simplify; i++)
						if (_childs[i]->getType() == TYPE_NODE ||
							_childs[i]->getLeaf() != value)
							simplify = false;
					if (simplify)
					{
						for (size_t i = 0; i < 4; i++)
							delete _childs[i];
						_type = TYPE_LEAF;
						_leaf = value;
					}
				}
				else
					throw (exoway::BasicException("node with unassigned child"));
			}
			else if (_type == TYPE_LEAF)
			{
				if (_size == 1)
					_leaf = value;
				else if (_leaf != value)
				{
					_type = TYPE_NODE;
					if (pos.x < _pos.x + _size / 2)
						index += CHILD_X_MIN;
					else
						index += CHILD_X_MAX;
					if (pos.y < _pos.y + _size / 2)
						index += CHILD_Y_MIN;
					else
						index += CHILD_Y_MAX;
					for (size_t i = 0; i < 4; i++)
						_childs[i] = new Quadtree<T>(
							glm::vec2(_pos.x + (((i & 1) * _size) >> 1), _pos.y + (((!((i & 2) >> 1)) * _size) >> 1)),
							_size / 2,
							_leaf,
							this);
					_childs[index]->privateSet(pos, value);
				}
			}
			else
				throw (exoway::BasicException("unknown quadtree type"));
		}

		void		privateDebug(size_t level)
		{
			for (size_t i = 0; i < level; i++)
				_log.debug << " ";
			if (_type == TYPE_NODE)
			{
				_log.debug << "{" << std::endl;
				for (size_t j = 0; j < 4; j++)
					_childs[j]->privateDebug(level + 2);
				for (size_t k = 0; k < level; k++)
					_log.debug << " ";
				_log.debug << "}" << std::endl;
			}
			else if (_type == TYPE_LEAF)
			{
				_log.debug << "(" << _pos.x << ";" << _pos.y << ") " << _leaf << std::endl;
			}
		}

		enum type	_type;
		Quadtree<T>	*_parent;
		Quadtree<T>	*_childs[4];
		T			_leaf;
		glm::vec2	_pos;
		size_t		_size;
};
