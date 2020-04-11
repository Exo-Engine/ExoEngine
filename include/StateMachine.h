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

#include <initializer_list>
#include <map>
#include <atomic>
#include <stack>

#include "State.h"
#include "Log.h"

namespace	ExoEngine
{

template	<typename ... Types>
class	StateMachine
{
	public:
		StateMachine(std::initializer_list<State<Types ...> *> states) : _state(0), _asyncRequest(0), _reloadRequest(false)
		{
			int	index = 0;

			for
				(State<Types ...>* state : states)
			{
				_states[index] = state;
				index++;
			}
			try
			{
				_states.at(_state)->load();
			}
			catch (const std::exception&)
			{
			}
		}
		virtual ~StateMachine(void)
		{
			try
			{
				_states.at(_state)->unload();
			}
			catch (const std::exception&)
			{
			}
		}

		void				switchTo(int state)
		{
			if (state == _state)
				return ;
			_asyncRequest = state;
			_states.at(state)->load();
			_states.at(_state)->unload();
			_state = state;
		}
		void				switchTo(const std::string &stateName)
		{
			for (std::pair<int, State<Types ...>*>& pair : _states)
				if (pair.second->name() == stateName)
					return switchTo(pair.first);
		}
		void				asyncSwitchTo(int state)
		{
			_asyncRequest = state;
		}
		void				asyncSwitchTo(const std::string &stateName)
		{
			for (const std::pair<int, State<Types ...>*>& pair : _states)
				if (pair.second->name() == stateName)
					return asyncSwitchTo(pair.first);
		}

		void				reload(void)
		{
			_states.at(_state)->unload();
			_states.at(_state)->load();
			_reloadRequest = false;
		}
		void				asyncReload(void)
		{
			_reloadRequest = true;
		}

		virtual void		run(Types ... args)
		{
			int	requested;

			if ((requested = _asyncRequest) != _state)
				switchTo(requested);
			if (_reloadRequest)
				reload();
			_states.at(_state)->run(args...);
		}
		int					getStateIndex(void) const
		{
			return (_state);
		}
		State<Types ...>	*getState(void) const
		{
			return (_states.at(_state));
		}

		void				addState(int index, State<Types ...>* state)
		{
			if (_states.find(index) == _states.end())
			{
				_states[index] = state;
				_state = index;
				state->load();
			}
			else
			{
				_states[index] = state;
			}
		}

		void				removeState(int index)
		{
			if (index == _state)
			{
				if (!_states.empty())
				{
					_state = _states.begin()->first;
				}
				else
				{
					_state = 0;
				}
			}
			_states.erase(_states.find(index));
		}

		void				removeState(State<Types ...>* state)
		{
			for (const std::pair<int, State<Types ...>*>& pair : _states)
				if (pair.second == state)
					return removeState(pair.first);
		}

	private:
		std::map<int, State<Types ...> *>	_states;
		std::atomic<int>					_state;
		std::atomic<int>					_asyncRequest;
		std::atomic<bool>					_reloadRequest;
};

}
