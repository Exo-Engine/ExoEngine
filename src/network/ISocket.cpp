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

#include "network/ISocket.h"
#include "Log.h"

using namespace	ExoEngine;
using namespace	network;

ISocket::ISocket(size_t size)
{
	_mutex.lock();

	static bool	initialized = false;
	int			result;

	if (initialized == false)
	{
		if (!SDL_WasInit(SDL_INIT_EVERYTHING))
		{
			result = SDL_Init(SDL_INIT_EVERYTHING);
			if (result)
			{
				_log.error << "cannot initialize SDL2: " << SDL_GetError() << std::endl;
				_mutex.unlock();
				throw (std::runtime_error(SDL_GetError()));
			}
			_log.debug << "SDL2 initialized" << std::endl;
		}
		result = SDLNet_Init();
		if (result)
		{
			_log.error << "cannot initialize SDLNet: " << SDL_GetError() << std::endl;
			_mutex.unlock();
			throw (std::runtime_error(SDL_GetError()));
		}
		initialized = true;
	}
	_binded = false;
	_set = SDLNet_AllocSocketSet((int)size);
	_clients_max = size;
	if (!_set)
	{
		_mutex.unlock();
		throw (std::runtime_error(std::string("cannot allocate socket set: ").append(SDLNet_GetError())));
	}
	_clientAddCb = NULL;
	_clientDelCb = NULL;
	_clientExceptionCb = NULL;
	_messageSendCb = NULL;
	_messageReceiveCb = NULL;
	_socketBindCb = NULL;
	_socketUnbindCb = NULL;
	_socketExceptionCb = NULL;

	_mutex.unlock();
}

ISocket::~ISocket(void)
{
	_mutex.lock();

	SDLNet_FreeSocketSet(_set);

	_mutex.unlock();
}

size_t	ISocket::getClientsNumber(void)
{
	size_t	size;

	_mutex.lock();

	size = _clients.size();

	_mutex.unlock();
	return (size);
}

bool	ISocket::isBind(void)
{
	bool	tmp;

	_mutex.lock();

	tmp = _binded;

	_mutex.unlock();
	return (tmp);
}

void	ISocket::setTimeout(Uint32 timeout)
{
	_mutex.lock();

	_timeout = timeout;

	_mutex.unlock();
}

void	ISocket::attachData(void *data)
{
	_mutex.lock();

	_data = data;

	_mutex.unlock();
}

void	*ISocket::getData(void)
{
	void	*tmp;

	_mutex.lock();

	tmp = _data;

	_mutex.unlock();
	return (tmp);
}

void	ISocket::setClientAddCb(void(*callback)(ISocket &, IClient *))
{
	_mutex.lock();

	_clientAddCb = callback;

	_mutex.unlock();
}

void	ISocket::setClientDelCb(void(*callback)(ISocket &, IClient *))
{
	_mutex.lock();

	_clientDelCb = callback;

	_mutex.unlock();
}

void	ISocket::setClientExceptionCb(void(*callback)(ISocket &, IClient *))
{
	_mutex.lock();

	_clientExceptionCb = callback;

	_mutex.unlock();
}

void	ISocket::setMessageSendCb(void(*callback)(ISocket &, IClient *, const Message &))
{
	_mutex.lock();

	_messageSendCb = callback;

	_mutex.unlock();
}

void	ISocket::setMessageReceiveCb(void(*callback)(ISocket &, IClient *, const Message &))
{
	_mutex.lock();

	_messageReceiveCb = callback;

	_mutex.unlock();
}

void	ISocket::setSocketBindCb(void(*callback)(ISocket &, uint16_t port))
{
	_mutex.lock();

	_socketBindCb = callback;

	_mutex.unlock();
}

void	ISocket::setSocketUnbindCb(void(*callback)(ISocket &, uint16_t port))
{
	_mutex.lock();

	_socketUnbindCb = callback;

	_mutex.unlock();
}

void	ISocket::setSocketExceptionCb(void(*callback)(ISocket &))
{
	_mutex.lock();

	_socketExceptionCb = callback;

	_mutex.unlock();
}
