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

#include <initializer_list>
#include <vector>
#include <atomic>
#include <stack>

#include "State.h"
#include "Log.h"

template    <typename ... Types>
class   StateMachine
{
    public:
        StateMachine(std::initializer_list<State<Types ...> *> states) : _states(states), _state(0), _asyncRequest(0), _reloadRequest(false)
        {
            _states[_state]->load(*this);
        }
        ~StateMachine(void)
        {
            _states[_state]->unload(*this);
        }

        void    switchTo(size_t state)
        {
            if (state == _state)
                return ;
            if (state >= _states.size())
            {
                _log.error << "cannot switch to state " << state << ", state machine have only " << _states.size() << std::endl;
                return ;
            }
            _log.debug << "switching from state " << _states[_state]->name() << " to state " << _states[state]->name() << std::endl;
            _asyncRequest = state;
            _states[state]->load(*this);
            _states[_state]->unload(*this);
            _state = state;
            _log.debug << "state switched" << std::endl;
        }
        void    switchTo(const std::string &state)
        {
            for (size_t i = 0; i < _states.size(); i++)
                if (_states[i]->name() == state)
                    return switchTo(i);
            _log.error << "cannot switch to state " << state << ", unknown state" << std::endl;
        }
        void    asyncSwitchTo(size_t state)
        {
            _asyncRequest = state;
        }
        void    asyncSwitchTo(const std::string &state)
        {
            for (size_t i = 0; i < _states.size(); i++)
                if (_states[i]->name() == state)
                    return asyncSwitchTo(i);
            _log.error << "cannot switch to state " << state << ", unknown state" << std::endl;
        }
    
        void    reload(void)
        {
            _log.debug << "reloaded from state " << _states[_state]->name() << " to state " << _states[_state]->name() << std::endl;
            _states[_state]->unload(*this);
            _states[_state]->load(*this);
            _reloadRequest = false;
            _log.debug << "state reloaded" << std::endl;
        }
        void    asyncReload(void)
        {
            _reloadRequest = true;
        }
    
        void    run(Types ... args)
        {
            size_t  requested;

            if ((requested = _asyncRequest) != _state)
                switchTo(requested);
            if (_reloadRequest)
                reload();
            _states[_state]->run(*this, args...);
        }
        size_t              getStateIndex(void) const
        {
            return (_state);
        }
        State<Types ...>    *getState(void) const
        {
            return (_states[_state]);
        }

        void    push(void *ptr)
        {
            _stack.push(ptr);
        }

        void    *pop(void)
        {
            void    *tmp;

            tmp = _stack.top();
            _stack.pop();
            return (tmp);
        }

        size_t  stackSize(void) const
        {
            return (_stack.size());
        }

    private:
        std::vector<State<Types ...> *> _states;
        std::atomic<size_t>             _state;
        std::atomic<size_t>             _asyncRequest;
        std::atomic<bool>               _reloadRequest;
        std::stack<void *>              _stack;
};
