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

#include "network/UdpClient.h"
#include "network/network.h"
#include "Log.h"

using namespace	ExoEngine;
using namespace	network;

UdpClient::UdpClient(const UDPsocket &socket, const IPaddress &address) : _socket(socket), _address(address)
{
}

UdpClient::~UdpClient(void)
{
}

const IPaddress		&UdpClient::getAddress(void) const
{
	return (_address);
}

std::string		UdpClient::getStrAddress(void) const
{
	std::string	str;
	Uint32	address = _address.host;
	SDLNet_Read32(&address);

	for (size_t i = 0; i < sizeof(address); i++)
		if (!i)
			str.append(std::to_string((Uint32)((Uint8 *)&address)[i] & 0xff));
		else
			str.append(".").append(std::to_string((Uint32)((Uint8 *)&address)[i] & 0xff));
	return (str);
}

std::string		UdpClient::getStrPort(void) const
{
	return (std::to_string(SDLNet_Read16(&_address.port)));
}

std::string		UdpClient::getHost(void) const
{
	return (std::string(SDLNet_ResolveIP(&_address)));
}

void	UdpClient::updateAddress(const IPaddress &address)
{
	_address = address;
}

SDLNet_GenericSocket	&UdpClient::getSocket(void)
{
	return ((SDLNet_GenericSocket &)_socket);
}

bool	UdpClient::operator==(const IPaddress &address) const
{
	return ((SDLNet_Read32(&address.host) == SDLNet_Read32(&_address.host) &&
			SDLNet_Read16(&address.port) == SDLNet_Read16(&_address.port)));
}

bool	UdpClient::operator==(const IClient &client) const
{
	return (*this == client.getAddress());
}
