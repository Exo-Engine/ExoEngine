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

#include <stdint.h>
#include <vector>
#include <map>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace	ExoEngine
{

namespace	network
{
	/*
	 *	endian function, will set a variable in good endianness if necessary
	 *	must be used for input/output operations (network, files etc...)
	 */
	template	<typename T>
	T	endian(const T &src)
	{
#if defined(sparc) || defined(mips) || defined(__arm__)
		T		dest;
		size_t	i = 0;
		size_t	j = sizeof(T) - 1;

		for (; i < sizeof(T); )
			((uint8_t *)(&dest))[i++] = ((uint8_t *)(&src))[j--];
		return (dest);
#else
		return (src);
#endif
	}

	#define ENDIAN(n)	n = network::endian(n)

	/*
	**	possible types of network packets
	*/

	typedef enum	e_type
	{
		TYPE_INVALID_PACKET_TYPE,
		TYPE_INVALID_PACKET_SIZE
	}				t_type;

	/*
	 *	each packet starts with a t_header structure,
	 *	containing the packet size and type.
	 *	The type is used to deduce the following data type.
	 */

	typedef struct	s_header
	{
		int32_t		type;
		uint32_t	size;
	}				t_header;

#define PAQUET_MAX_SIZE	8192

	/*
	 *	TYPE_INVALID_PACKET_TYPE
	 *
	 *	error packet, received after sended a packet with an invalid type
	 *
	 *	header.size = sizeof(t_header)
	 */

	/*
	 *	TYPE_INVALID_PACKET_SIZE
	 *
	 *	error packet, sent after reception of a packet with invalid size.
	 *
	 *	header.size = sizeof(t_header)
	 */

};

}
