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

#include <stdexcept>

namespace	ExoEngine
{

template	<typename T, size_t S>
class		CircularBuffer
{
		public:
			CircularBuffer(void) : _read(0), _write(0), _n(0)
			{
				if (!S)
					throw (std::invalid_argument("CircularBuffer cannot have a null size"));
			}
			~CircularBuffer(void) noexcept
			{
			}

			void	push(const T &src) noexcept
			{
				_buffer[_write] = src;
				_write = (_write + 1) % S;
				if (_n < S)
					_n++;
			}
			T		pop(void)
			{
				if (!_n)
					throw (std::logic_error("nothing to pop"));

				T	&tmp(_buffer[_read]);

				_read = (_read + 1) % S;
				_n--;
				return (tmp);
			}
			bool	isEmpty(void) const noexcept
			{
				return ((_n) ? false : true);
			}

			size_t	size(void) const
			{
				return (_n);
			}

			void	clear(void) noexcept
			{
				_read = 0;
				_write = 0;
				_n = 0;
			}

			const T	&operator[](const size_t &index)
			{
				if (index >= _n)
					throw (std::out_of_range(__FUNCTION__));
				return (_buffer[(_read + index) % S]);
			}
		private:
			T		_buffer[S];
			size_t	_read;
			size_t	_write;
			size_t	_n;
};

}
