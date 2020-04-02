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

#include "network/TcpSocket.h"
#include "network/TcpClient.h"
#include "Log.h"

using namespace	ExoEngine;
using namespace	network;

TcpSocket::TcpSocket(size_t size) : ISocket(size)
{
}

TcpSocket::~TcpSocket(void)
{
	_mutex.lock();

	if (_binded)
		unbind();
	for (size_t i = 0; i < _clients.size(); i++)
	{
		try
		{
			disconnect(_clients[i]);
		}
		catch (const std::exception &e)
		{
			_log.error << "error while deleting socket: " << e.what() << std::endl;
		}
	}

	_mutex.unlock();
}

void	TcpSocket::bind(uint16_t port)
{
	_mutex.lock();

	IPaddress	ip;

	if (isBind())
	{
		_mutex.unlock();
		throw (std::runtime_error(std::string("cannot bind socket to ").append(std::to_string(port)).append(": socket already bind")));
	}
	if (SDLNet_ResolveHost(&ip, NULL, port))
	{
		_mutex.unlock();
		throw (std::runtime_error(std::string("cannot bind socket to ").append(std::to_string(port)).append(SDLNet_GetError())));
	}
	_socket = SDLNet_TCP_Open(&ip);
	if (!_socket)
	{
		_mutex.unlock();
		throw (std::runtime_error(std::string("cannot bind socket to ").append(std::to_string(port)).append(SDLNet_GetError())));
	}
	if (SDLNet_TCP_AddSocket(_set, _socket) == -1)
	{
		_mutex.unlock();
		throw (std::runtime_error(std::string("cannot add binded socket to set: ").append(SDLNet_GetError())));
	}
	_binded = true;
	_port = port;
	if (_socketBindCb)
		_socketBindCb(*this, port);

	_mutex.unlock();
}

void	TcpSocket::bind(const std::string &port)
{
	size_t	tmp;

	tmp = std::stoul(port);
	if (tmp > 0xffff)
		throw (std::runtime_error(std::string("cannot bind socket to ").append(port).append(": port greater than 65535")));
	bind((uint16_t)tmp);
}

void	TcpSocket::unbind(void)
{
	_mutex.lock();

	if (!_binded)
	{
		_mutex.unlock();
		throw (std::runtime_error("cannot unbind when server isn't bind"));
	}
	if (SDLNet_TCP_DelSocket(_set, _socket) == -1)
	{
		_mutex.unlock();
		throw (std::runtime_error(std::string("cannot remove binded socket from set: ").append(SDLNet_GetError())));
	}
	SDLNet_TCP_Close(_socket);
	_binded = false;
	if (_socketUnbindCb)
		_socketUnbindCb(*this, _port);

	_mutex.unlock();
}

void	TcpSocket::connect(const std::string &address, const std::string &port)
{
	size_t		tmp;

	tmp = std::stoul(port);
	if (tmp > 0xffff)
		throw (std::runtime_error(std::string("cannot connect socket to ").append(address).append(":").append(std::to_string(tmp)).append(": port greater than 65535")));
	connect(address, (uint16_t)tmp);
}

void	TcpSocket::connect(const std::string &address, uint16_t port)
{
	_mutex.lock();

	IClient		*newClient;
	TCPsocket	socket;
	IPaddress	ip;

	if (SDLNet_ResolveHost(&ip, address.c_str(), port) != 0)
	{
		_mutex.unlock();
		throw (std::runtime_error(std::string("cannot resolve ").append(address).append(":").append(std::to_string(port))));
	}
	for (auto client = _clients.begin(); client != _clients.end(); client++)
		if (**client == ip)
		{
			_mutex.unlock();
			throw (std::runtime_error("client already added"));
		}
	socket = SDLNet_TCP_Open(&ip);
	if (!socket)
	{
		_mutex.unlock();
		throw (std::runtime_error(std::string("cannot connect socket to ").append(address).append(":").append(std::to_string(port)).append(SDLNet_GetError())));
	}
	if (SDLNet_TCP_AddSocket(_set, socket) == -1)
	{
		_mutex.unlock();
		throw (std::runtime_error(std::string("cannot add binded socket to set: ").append(SDLNet_GetError())));
	}
	newClient = new TcpClient(socket);
	_clients.push_back(newClient);
	if (_clientAddCb)
		_clientAddCb(*this, _clients.back());
	_mutex.unlock();
}

void	TcpSocket::disconnect(IClient *client)
{
	_mutex.lock();

	for (auto tmp = _clients.begin(); tmp != _clients.end(); tmp++)
		if (client == *tmp)
		{
			if (SDLNet_DelSocket(_set, client->getSocket()) == -1)
			{
				_mutex.unlock();
				throw (std::runtime_error(std::string("cannot remove client socket from set: ").append(SDLNet_GetError())));
			}
			if (_clientDelCb)
				_clientDelCb(*this, client);
			delete client;
			_clients.erase(tmp);
			break ;
		}
	_mutex.unlock();
}

void	TcpSocket::pollEvent(uint8_t mask)
{
	TCPsocket			new_socket;
	IClient				*new_client;
	int					ret;

	_mutex.lock();

	(void)mask;
	if (!_binded && !_clients.size())
		return (_mutex.unlock());
	ret = SDLNet_CheckSockets(_set, _timeout);
	if (ret == -1)
	{
		_mutex.unlock();
		throw (std::runtime_error(std::string("socket check failed: ").append(SDLNet_GetError())));
	}
	if (ret > 0)
	{
		while (ret > 0 && isBind() && SDLNet_SocketReady(getSocket()))
		{
			new_socket = SDLNet_TCP_Accept(_socket);
			if (!new_socket)
			{
				_mutex.unlock();
				throw (std::runtime_error(std::string("cannot get incoming client: ").append(SDLNet_GetError())));
			}
			new_client = new TcpClient(new_socket);
			if (!new_client)
			{
				_mutex.unlock();
				throw (std::runtime_error("cannot get incoming client: ran out of memory"));
			}
			if (SDLNet_TCP_AddSocket(_set, new_socket) == -1)
			{
				delete new_client;
				_mutex.unlock();
				throw (std::runtime_error(std::string("cannot add client socket to set: ").append(SDLNet_GetError())));
			}
			_clients.push_back(new_client);
			if (_clientAddCb)
				_clientAddCb(*this, _clients.back());
			ret--;
		}
		for (auto client = _clients.begin(); client != _clients.end() && ret > 0; client++)
			while (ret > 0 && SDLNet_SocketReady((*client)->getSocket()))
			{
				Message	message;
				char	buffer[SOCKET_READ_BUFFER_SIZE];
				int		read;

				read = SDLNet_TCP_Recv((TCPsocket)(*client)->getSocket(), (void *)buffer, SOCKET_READ_BUFFER_SIZE);
				if (!read)
				{
					if (SDLNet_DelSocket(_set, (*client)->getSocket()) == -1)
					{
						_mutex.unlock();
						throw (std::runtime_error(std::string("cannot remove client socket from set: ").append(SDLNet_GetError())));
					}
					if (_clientDelCb)
						_clientDelCb(*this, (*client));
					_clients.erase(client);
				}
				else if (read > 0)
				{
					message.append((void *)buffer, read);
					if (_messageReceiveCb)
						_messageReceiveCb(*this, (*client), message);
				}
				else
				{
					if (_clientExceptionCb)
						_clientExceptionCb(*this, (*client));
				}
				ret--;
			}
	}
	_mutex.unlock();
}

void	TcpSocket::send(IClient *client, const Message &message)
{
	_mutex.lock();

	int	ret;

	if (!client)
	{
		_log.error << __FUNCTION__ << " client NULL" << std::endl;
		_mutex.unlock();
		return ;
	}
	ret = SDLNet_TCP_Send((TCPsocket)client->getSocket(), message.getPtr(), (int)message.getSize());
	if (ret < (int)message.getSize() && _clientExceptionCb)
		_clientExceptionCb(*this, client);
	if (_messageSendCb)
		_messageSendCb(*this, client, message);
	_mutex.unlock();
}

SDLNet_GenericSocket	TcpSocket::getSocket(void)
{
	SDLNet_GenericSocket	tmp;

	_mutex.lock();

	tmp = (SDLNet_GenericSocket)_socket;

	_mutex.unlock();
	return (tmp);
}

ISocket::type	TcpSocket::getType(void) const
{
	return (TCP);
}
