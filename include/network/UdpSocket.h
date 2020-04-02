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

#include "network/ISocket.h"

namespace	ExoEngine
{

namespace	network
{

class	UdpSocket : public ISocket
{
	public:
		UdpSocket(size_t size);
		~UdpSocket(void);

		virtual void	bind(uint16_t port);
		virtual void	bind(const std::string &port);
		virtual void	unbind(void);
		virtual void	connect(const std::string &address, const std::string &port);
		virtual void	connect(const std::string &address, uint16_t port);
		virtual void	disconnect(IClient *client);
		virtual void	pollEvent(uint8_t mask);
		virtual void	send(IClient *client, const Message &message);

		virtual SDLNet_GenericSocket	getSocket(void);
		virtual type	getType(void) const;
	private:
		UDPsocket	_socket;
		UDPpacket	*_packet;
};

}

}
