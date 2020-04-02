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

#include "network/UdpSocket.h"
#include "network/UdpClient.h"
#include "Log.h"

using namespace	ExoEngine;
using namespace	network;

UdpSocket::UdpSocket(size_t size) : ISocket(size)
{
	_mutex.lock();

	_packet = SDLNet_AllocPacket(SOCKET_READ_BUFFER_SIZE);
	if (!_packet)
	{
		_mutex.unlock();
		throw (std::runtime_error("cannot allocate enough space for udp packet buffer"));
	}
	_mutex.unlock();
}

UdpSocket::~UdpSocket(void)
{
	_mutex.lock();

	if (_binded)
		unbind();
	for (size_t i = 0; i < _clients.size(); i++)
		disconnect(_clients[i]);

	_mutex.unlock();
}

void	UdpSocket::bind(uint16_t port)
{
	_mutex.lock();

	if (isBind())
	{
		_mutex.unlock();
		throw (std::runtime_error(std::string("cannot bind socket to ").append(std::to_string(port)).append(": socket already bind")));
	}
	_socket = SDLNet_UDP_Open(port);
	if (!_socket)
	{
		_mutex.unlock();
		throw (std::runtime_error(std::string("cannot bind socket to ").append(std::to_string(port)).append(SDLNet_GetError())));
	}
	if (SDLNet_UDP_AddSocket(_set, _socket) == -1)
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

void	UdpSocket::bind(const std::string &port)
{
	size_t	tmp;

	tmp = std::stoul(port);
	if (tmp > 0xffff)
		throw (std::runtime_error(std::string("cannot bind socket to ").append(port).append(": port greater than 65535")));
	bind((uint16_t)tmp);
}

void	UdpSocket::unbind(void)
{
	_mutex.lock();

	if (!_binded)
	{
		_mutex.unlock();
		throw (std::runtime_error("cannot unbind when server isn't bind"));
	}
	if (SDLNet_UDP_DelSocket(_set, _socket) == -1)
	{
		_mutex.unlock();
		throw (std::runtime_error(std::string("cannot remove binded socket from set: ").append(SDLNet_GetError())));
	}
	SDLNet_UDP_Close(_socket);
	_binded = false;
	if (_socketUnbindCb)
		_socketUnbindCb(*this, _port);
	_mutex.unlock();
}

void	UdpSocket::connect(const std::string &address, const std::string &port)
{
	size_t		tmp;

	tmp = std::stoul(port);
	if (tmp > 0xffff)
		throw (std::runtime_error(std::string("cannot connect socket to ").append(address).append(":").append(std::to_string(tmp)).append(": port greater than 65535")));
	connect(address, (uint16_t)tmp);
}

void	UdpSocket::connect(const std::string &address, uint16_t port)
{
	UDPsocket	newSocket;
	UdpClient	*newClient;
	IPaddress	ip;
	int			ret;

	_mutex.lock();

	if (SDLNet_ResolveHost(&ip, address.c_str(), port))
	{
		_mutex.unlock();
		throw (std::runtime_error(std::string("cannot connect to ").append(address).append(":").append(std::to_string(port)).append(" : ").append(SDLNet_GetError())));
	}
	for (auto client = _clients.begin(); client != _clients.end(); client++)
		if (**client == ip)
		{
			_mutex.unlock();
			throw (std::runtime_error("client already added"));
		}
	newSocket = SDLNet_UDP_Open(0);
	if (!newSocket)
	{
		_mutex.unlock();
		throw (std::runtime_error(std::string("cannot create udp socket: ").append(SDLNet_GetError())));
	}
	ret = SDLNet_UDP_Bind(newSocket, -1, &ip);
	if (ret == -1)
	{
		_mutex.unlock();
		throw (std::runtime_error(std::string("cannot bind udp socket to ip address: ").append(SDLNet_GetError())));
	}
	if (SDLNet_UDP_AddSocket(_set, newSocket) == -1)
	{
		_mutex.unlock();
		throw (std::runtime_error(std::string("cannot add socket to set: ").append(SDLNet_GetError())));
	}
	newClient = new UdpClient(newSocket, ip);
	_clients.push_back(newClient);
	if (_clientAddCb)
		_clientAddCb(*this, newClient);
	_mutex.unlock();
}

void	UdpSocket::disconnect(IClient *client)
{
	_mutex.lock();

	for (auto tmp = _clients.begin(); tmp != _clients.end(); tmp++)
		if (client == *tmp)
		{
			if (_clientDelCb)
				_clientDelCb(*this, client);
			delete client;
			_clients.erase(tmp);
			break ;
		}

	_mutex.unlock();
}

void	UdpSocket::pollEvent(uint8_t mask)
{
	IClient		*new_client;
	int			ret, ret2;
	bool		found;

	_mutex.lock();

	if (!_binded && !_clients.size())
		return _mutex.unlock();
	(void)mask;
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
			ret2 = SDLNet_UDP_Recv(_socket, _packet);
			if (ret2 == 1)
			{
				found = false;
				for (size_t i = 0; i < _clients.size(); i++)
					if (_clients[i]->getAddress().host == _packet->address.host)
					{
						_clients[i]->updateAddress(_packet->address);
						if (_messageReceiveCb)
							_messageReceiveCb(*this, _clients[i], Message(_packet->data, _packet->len));
						found = true;
						break ;
					}
				if (!found)
				{
					new_client = new UdpClient(_socket, _packet->address);
					_clients.push_back(new_client);
					if (_clientAddCb)
						_clientAddCb(*this, new_client);
					if (_messageReceiveCb)
						_messageReceiveCb(*this, new_client, Message(_packet->data, _packet->len));
				}
			}
			else if (ret2 == -1)
			{
				_mutex.unlock();
				throw (std::runtime_error(std::string("error while receiving udp packet: ").append(SDLNet_GetError())));
			}
			ret--;
		}
		for (size_t i = 0; i < _clients.size() && ret > 0; i++)
			while (ret > 0 && SDLNet_SocketReady(_clients[i]->getSocket()))
			{
				ret2 = SDLNet_UDP_Recv((UDPsocket)_clients[i]->getSocket(), _packet);
				if (ret2 == 1)
				{
					if (_messageReceiveCb)
						_messageReceiveCb(*this, _clients[i], Message(_packet->data, _packet->len));
				}
				else if (ret2 == -1)
				{
					_mutex.unlock();
					throw (std::runtime_error(std::string("error while receiving udp packet: ").append(SDLNet_GetError())));
				}
				ret--;
			}
	}

	_mutex.unlock();
}

void	UdpSocket::send(IClient *client, const Message &message)
{
	UDPpacket	packet;

	_mutex.lock();

	if (!client)
	{
		_log.error << __FUNCTION__ << " client NULL" << std::endl;
		_mutex.unlock();
		return ;
	}
	packet.channel = -1;
	packet.data = (Uint8 *)message.getPtr();
	packet.len = (int)message.getSize();
	packet.maxlen = (int)message.getSize();
	packet.address = client->getAddress();
	if (SDLNet_UDP_Send((UDPsocket)client->getSocket(), -1, &packet))
		_messageSendCb(*this, client, message);
	else
	{
		_mutex.unlock();
		throw (std::runtime_error(std::string("cannot send udp packet: ").append(SDLNet_GetError())));
	}

	_mutex.unlock();
}

SDLNet_GenericSocket	UdpSocket::getSocket(void)
{
	SDLNet_GenericSocket	tmp;

	_mutex.lock();

	tmp = (SDLNet_GenericSocket)_socket;

	_mutex.unlock();
	return (tmp);
}

ISocket::type	UdpSocket::getType(void) const
{
	return (UDP);
}
