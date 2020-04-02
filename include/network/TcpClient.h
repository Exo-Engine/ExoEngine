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

#include "network/IClient.h"

namespace	ExoEngine
{

namespace	network
{

class TcpClient : public virtual IClient
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
		virtual bool	operator==(const IClient &client) const;
	private:
		TCPsocket	_socket;
		IPaddress	*_address;
};

}

}
