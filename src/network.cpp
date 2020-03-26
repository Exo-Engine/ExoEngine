/*
 *  MIT License
 *  
 *  Copyright (c) 2020 Gaëtan Dezeiraud and Ribault Paul
 *  
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *  
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *  
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */

#include "network.h"

#define PAIR_ENUM(e)	std::make_pair(e, #e)

const std::string	&network::getStrType(const t_type &type)
{
	static const std::map<t_type, std::string>	map({
		PAIR_ENUM(TYPE_INVALID_PACKET_TYPE),
		PAIR_ENUM(TYPE_INVALID_PACKET_SIZE),
		PAIR_ENUM(TYPE_INVALID_STATE),
		PAIR_ENUM(TYPE_DISCOVER_REQUEST),
		PAIR_ENUM(TYPE_SERVER_PROPERTIES),
		PAIR_ENUM(TYPE_CONNECT_REQUEST),
		PAIR_ENUM(TYPE_INCOMPATIBLE_VERSION),
		PAIR_ENUM(TYPE_SERVER_FULL),
		PAIR_ENUM(TYPE_INVALID_RSA_KEY),
		PAIR_ENUM(TYPE_AUTH_REQUEST),
		PAIR_ENUM(TYPE_AUTH),
		PAIR_ENUM(TYPE_CONNEXION_ACCEPTED),
		PAIR_ENUM(TYPE_CONNEXION_REFUSED),
        PAIR_ENUM(TYPE_SEND_CAMERA),
        PAIR_ENUM(TYPE_SEND_METADATA),
		PAIR_ENUM(TYPE_ADD_OBJECT),
		PAIR_ENUM(TYPE_DEL_OBJECT),
		PAIR_ENUM(TYPE_ADD_PLAYER),
		PAIR_ENUM(TYPE_DEL_PLAYER),
		PAIR_ENUM(TYPE_UPDATE_OBJECT_POS),
		PAIR_ENUM(TYPE_UPDATE_OBJECT_SCALE),
		PAIR_ENUM(TYPE_UPDATE_OBJECT_SPEED),
		PAIR_ENUM(TYPE_UPDATE_OBJECT_ANGLE),
		PAIR_ENUM(TYPE_UPDATE_OBJECT_ROT_SPEED),
		PAIR_ENUM(TYPE_GLOBAL_MESSAGE),
		PAIR_ENUM(TYPE_PRIVATE_MESSAGE),
		PAIR_ENUM(TYPE_OBJECT_SAY),
		PAIR_ENUM(TYPE_OBJECT_SET_SPRITE)
	});
	static std::string	tmp;

	try
	{
		return (map.at(type));
	}
	catch (const std::exception &)
	{
		tmp = "UNKNOWN(";
		return (tmp.append(std::to_string(type)).append(")"));
	}
}

const std::string	&network::getStrState(const t_state &state)
{
	static const std::map<t_state, std::string>	map({
		PAIR_ENUM(STATE_DISCONNECTED),
		PAIR_ENUM(STATE_CONNECTING),
		PAIR_ENUM(STATE_CONNECTED),
		PAIR_ENUM(STATE_DISCONNECTING)
	});
	static std::string	tmp;

	try
	{
		return (map.at(state));
	}
	catch (const std::exception &)
	{
		tmp = "UNKNOWN(";
		return (tmp.append(std::to_string(state)).append(")"));
	}
}
