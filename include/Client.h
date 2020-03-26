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

#include "Message.h"

#ifndef SOCKET_BUFFER_SIZE
# define SOCKET_BUFFER_SIZE	1024
#endif

#define SOCKET_ALLOW_CONNECTIONS	(1 << 0)
#define SOCKET_ALLOW_READ			(1 << 1)
#define SOCKET_ALLOW_WRITE			(1 << 2)

#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>

namespace	ExoEngine
{

class Client
{
	public:
		virtual ~Client(void);

		virtual const IPaddress	&getAddress(void) const = 0;
		virtual std::string		getStrAddress(void) const = 0;
		virtual std::string		getStrPort(void) const = 0;
		virtual std::string		getHost(void) const = 0;

		virtual SDLNet_GenericSocket	&getSocket(void) = 0;

		virtual void	updateAddress(const IPaddress &address) = 0;

		void			attachData(void *data);
		void			*getData(void);

		virtual bool	operator==(const IPaddress &address) const = 0;
		virtual bool	operator==(const Client &client) const = 0;
	private:
		void	*_data;
};

class TcpClient : public virtual Client
{
	public:
		TcpClient(const TCPsocket &socket);
		virtual ~TcpClient(void);

		virtual const IPaddress	&getAddress(void) const;
		virtual std::string		getStrAddress(void) const;
		virtual std::string		getStrPort(void) const;
		virtual std::string		getHost(void) const;

		virtual SDLNet_GenericSocket	&getSocket(void);

		virtual void	updateAddress(const IPaddress &address);

		virtual bool	operator==(const IPaddress &address) const;
		virtual bool	operator==(const Client &client) const;
	private:
		TCPsocket	_socket;
		IPaddress	*_address;
};

class UdpClient : public virtual Client
{
	public:
		UdpClient(const UDPsocket &socket, const IPaddress &address);
		virtual ~UdpClient(void);

		virtual const IPaddress	&getAddress(void) const;
		virtual std::string		getStrAddress(void) const;
		virtual std::string		getStrPort(void) const;
		virtual std::string		getHost(void) const;

		virtual SDLNet_GenericSocket	&getSocket(void);

		virtual void	updateAddress(const IPaddress &address);

		virtual bool	operator==(const IPaddress &address) const;
		virtual bool	operator==(const Client &client) const;
	private:
		UDPsocket	_socket;
		IPaddress	_address;
};

}
