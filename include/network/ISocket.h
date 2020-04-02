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

#include <mutex>

#ifndef SOCKET_READ_BUFFER_SIZE
# define SOCKET_READ_BUFFER_SIZE	4096
#endif

namespace	ExoEngine
{

namespace	network
{

class	ISocket
{
	public:
		ISocket(size_t size);
		~ISocket(void);

		virtual void	bind(uint16_t port) = 0;
		virtual void	bind(const std::string &port) = 0;
		virtual void	unbind(void) = 0;
		virtual void	connect(const std::string &address, const std::string &port) = 0;
		virtual void	connect(const std::string &address, uint16_t port) = 0;
		virtual void	disconnect(IClient *client) = 0;
		virtual void	pollEvent(uint8_t mask) = 0;
		virtual void	send(IClient *client, const Message &message) = 0;

		virtual SDLNet_GenericSocket	getSocket(void) = 0;

		size_t	getClientsNumber(void);

		bool	isBind(void);
		void	setTimeout(Uint32 timeout);

		typedef enum
		{
			TCP,
			UDP
		}		type;
		virtual type	getType(void) const = 0;

		void	attachData(void *data);
		void	*getData(void);

		void	setClientAddCb(void(*callback)(ISocket &, IClient *));
		void	setClientDelCb(void(*callback)(ISocket &, IClient *));
		void	setClientExceptionCb(void(*callback)(ISocket &, IClient *));
		void	setMessageSendCb(void(*callback)(ISocket &, IClient *, const Message &));
		void	setMessageReceiveCb(void(*callback)(ISocket &, IClient *, const Message &));
		void	setSocketBindCb(void(*callback)(ISocket &, uint16_t port));
		void	setSocketUnbindCb(void(*callback)(ISocket &, uint16_t port));
		void	setSocketExceptionCb(void(*callback)(ISocket &));
	protected:
		std::recursive_mutex	_mutex;
		SDLNet_SocketSet		_set;
		size_t					_clients_max;
		std::vector<IClient *>	_clients;
		bool					_binded;
		uint16_t				_port;
		Uint32					_timeout;
		void					*_data;
		void					(*_clientAddCb)(ISocket &socket, IClient *client);
		void					(*_clientDelCb)(ISocket &socket, IClient *client);
		void					(*_clientExceptionCb)(ISocket &socket, IClient *client);
		void					(*_messageSendCb)(ISocket &socket, IClient *client, const Message &message);
		void					(*_messageReceiveCb)(ISocket &socket, IClient *client, const Message &message);
		void					(*_socketBindCb)(ISocket &socket, uint16_t port);
		void					(*_socketUnbindCb)(ISocket &socket, uint16_t port);
		void					(*_socketExceptionCb)(ISocket &socket);
};

}

}
