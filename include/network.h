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

#pragma once

#include <stdint.h>
#include <vector>
#include <map>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include "Socket.h"

namespace	network
{
	/*
	**	endian function, will set a variable in good endianness if necessary
	**	must be used for input/output operations (network, files etc...)
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
		TYPE_INVALID_PACKET_SIZE,
		TYPE_INVALID_STATE,
		TYPE_DISCOVER_REQUEST,
		TYPE_SERVER_PROPERTIES,
		TYPE_CONNECT_REQUEST,
		TYPE_INCOMPATIBLE_VERSION,
		TYPE_SERVER_FULL,
		TYPE_INVALID_RSA_KEY,
		TYPE_AUTH_REQUEST,
		TYPE_AUTH,
		TYPE_CONNEXION_ACCEPTED,
		TYPE_CONNEXION_REFUSED,
		TYPE_SEND_CAMERA,
		TYPE_SEND_METADATA,
		TYPE_ADD_OBJECT,
		TYPE_DEL_OBJECT,
		TYPE_ADD_PLAYER,
		TYPE_DEL_PLAYER,
		TYPE_UPDATE_OBJECT_POS,
		TYPE_UPDATE_OBJECT_SCALE,
		TYPE_UPDATE_OBJECT_SPEED,
		TYPE_UPDATE_OBJECT_ANGLE,
		TYPE_UPDATE_OBJECT_ROT_SPEED,
		TYPE_GLOBAL_MESSAGE,
		TYPE_PRIVATE_MESSAGE,
		TYPE_OBJECT_SAY,
		TYPE_OBJECT_SET_SPRITE,
		TYPE_MAX
	}				t_type;

	/*
	**	return str version of type enum
	*/

	const std::string	&getStrType(const t_type &type);

	/*
	**	states
	*/

	typedef enum	e_state
	{
		STATE_CONNECTING =		(1 << 0),
		STATE_CONNECTED =		(1 << 1),
		STATE_DISCONNECTING =	(1 << 2),
		STATE_DISCONNECTED =	(1 << 3)
	}				t_state;

	/*
	**	return str version of state enum
	*/

	const std::string	&getStrState(const t_state &state);

	/*
	**	each packet starts with a t_header structure,
	**	containing the packet size and type.
	**	The type is used to deduce the following data type.
	*/

	typedef struct	s_header
	{
		t_type		type;
		size_t		size;
	}				t_header;

#define PAQUET_MAX_SIZE	8192

	/*
	**	TYPE_INVALID_PACKET_TYPE
	**
	**	error packet, received after sended a packet with an invalid type
	**
	**	header.size = sizeof(t_header)
	*/

	/*
	**	TYPE_INVALID_PACKET_SIZE
	**
	**	error packet, sent after reception of a packet with invalid size.
	**
	**	header.size = sizeof(t_header)
	*/

	/*
	**	TYPE_INVALID_STATE
	**
	**	error packet, sent when you were'nt allowed to send a previous packet
	**
	**	header.size = sizeof(t_header) + sizeof(t_invalid_state)
	*/

	typedef struct	s_invalid_state
	{
		t_type		packet_type;
		t_state		current_state;
	}				t_invalid_state;

	/*
	**	TYPE_DISCOVER_REQUEST
	**
	**	sent by a client to a server to request informations.
	**	no data structure follows header:
	**	header.size = sizeof(t_header)
	*/

	/*
	**	TYPE_SERVER_PROPERTIES
	**
	**	sended by a server to a client in response of a discover request,
	**	containing informations about it.
	**	after this structure will follow the raw name of name_length bytes,
	**	and the raw description of description_length bytes.
	**	header.length = sizeof(t_header) + sizeof(t_server_properties) + properties.name_length + properties.description_length
	*/
	typedef struct	s_server_properties
	{
		uint32_t	version_major;
		uint32_t	version_minor;
		uint32_t	version_fix;
		uint32_t	clients;
		uint32_t	clients_max;
		uint32_t	name_length;
		uint32_t	description_length;
	}				t_server_properties;

	/*
	**	TYPE_CONNECT_REQUEST
	**
	**	sent by client to server to request connexion.
	**	After this header will follow a string of rsa_key_length
	**	bytes, allowing the server to secure the connexion communication.
	**
	**	server can respond with a TYPE_INVALID_RSA_KEY packet in case of an invalid rsa key,
	**	or a TYPE_AUTH_REQUEST packet to continue connexion process.
	**
	**	header.length = sizeof(t_header) + sizeof(t_connect_request) + connect_request.rsa_key_length
	*/

	typedef struct	s_connect_request
	{
		uint32_t	version_major;
		uint32_t	version_minor;
		uint32_t	version_fix;
		size_t		rsa_key_length;
	}				t_connect_request;

	/*
	**	TYPE_SERVER_FULL
	**
	**	error packet, nothing more
	**	header.length = sizeof(t_header)
	*/

	/*
	**	TYPE_INVALID_RSA_KEY
	**
	**	error packet, nothing more
	**	header.length = sizeof(t_header)
	*/

	/*
	**	TYPE_INCOMPATIBLE_VERSION
	**
	**	error packet, sent when version is incompatible with server's.
	**	The following packet will contain server's version.
	**
	**	header.length = sizeof(t_header) + sizeof(t_incompatible_version)
	*/

	typedef struct	s_incompatible_version
	{
		uint32_t	version_major;
		uint32_t	version_minor;
		uint32_t	version_fix;
	}				t_incompatible_version;

	/*
	**	TYPE_AUTH_REQUEST
	**
	**	sent by server to client after a TYPE_CONNECT_REQUEST.
	**	The server will use the rsa key given by client to encrypt and send the following data.
	**	server rsa public key being transmitted to each one requesting a connexion, a token is given
	**	to the client with an encrypted connexion to ensure the connexion author,
	**	this token being requested later in the TYPE_AUTH packet.
	**
	**	header.length = sizeof(t_header) + sizeof(t_auth_request) + auth_request.token_length + auth_request.rsa_key_length
	*/

	typedef struct	s_auth_request
	{
		size_t		token_length;
		size_t		rsa_key_length;
	}				t_auth_request;

	/*
	**	TYPE_AUTH
	**
	**	sent by client to server to authenticate himself.
	**	The following data will be encrypted with the server rsa public key,
	**	and will contain the token given previously by the server and the required auth informations.
	**
	**	header.length = sizeof(t_header) + sizeof(t_auth) + auth.name_length
	*/

	typedef struct	s_auth
	{
		size_t		token;
		size_t		name_length;
	}				t_auth;

	/*
	**	TYPE_CONNEXION_ACCEPTED
	**
	**	connexion accepted by server.
	*/

	/*
	**	TYPE_CONNEXION_REFUSED
	**
	**	connexion refused by server.
	**
	**	header.length = sizeof(t_header) + sizeof(t_connection_refused)
	*/

	typedef enum	e_connexion_refused_reason
	{
		REASON_INVALID_TOKEN
	}				t_connexion_refused_reason;

	typedef struct					s_connexion_refused
	{
		t_connexion_refused_reason	reason;
	}								t_connexion_refused;

	 /*
	 **	TYPE_SEND_CAMERA
	 **
	 **	sent by server to a client the camera parameters
	 **
	 **	header.size = sizeof(t_header) + sizeof(s_send_camera)
	 */

	typedef struct		s_send_camera
	{
		unsigned int	type;
		glm::vec3		pos;
	}					t_send_camera;

	 /*
	 **	TYPE_SEND_METADATA
	 **
	 **	sent by server to a client to send map data
	 **
	 **	header.size = sizeof(t_header) + sizeof(s_send_metadata) + send_metadata.id_length + send_metadata.value_length
	 */

	typedef struct		s_send_metadata
	{
		size_t			id_length;
		size_t			value_length;
	}					t_send_metadata;

	/*
	**	TYPE_ADD_OBJECT
	**
	**	sent by server to a client to notify a new object
	**
	**	header.size = sizeof(t_header) + sizeof(t_add_object)
	*/

	typedef struct	s_add_object
	{
		size_t		id;
		int			type;
		uint32_t	bitfield;
		glm::vec2	pos;
		glm::vec2	scale;
		glm::vec2	speed;
		float		angle;
		float		rotation_speed;
		size_t		resource_id;
	}				t_add_object;

	/*
	**	TYPE_DEL_OBJECT
	**
	**	sent by server to a client to notify object deletion
	**
	**	header.size = sizeof(t_header) + sizeof(t_del_object)
	*/

	typedef struct	s_del_object
	{
		size_t		id;
	}				t_del_object;

	/*
	**	TYPE_ADD_PLAYER
	**
	**	sent by server to client to add a player
	**
	**	header.size = sizeof(t_header) + sizeof(t_add_player) + add_player.name_length
	*/

	typedef struct	s_add_player
	{
		size_t		id;
		size_t		object_id;
		size_t		name_length;
	}				t_add_player;

	/*
	**	TYPE_DEL_PLAYER
	**
	**	header.size = sizeof(t_header) + sizeof(t_del_player)
	*/

	typedef struct	s_del_player
	{
		size_t		id;
	}				t_del_player;

	/*
	**	TYPE_UPDATE_OBJECT_POS
	**
	**	header.size = sizeof(t_header) + sizeof(t_update_object_pos)
	*/

	typedef struct	s_update_object_pos
	{
		size_t		object_id;
		glm::vec2	pos;
	}				t_update_object_pos;

	/*
	**	TYPE_UPDATE_OBJECT_SPEED
	**
	**	header.size = sizeof(t_header) + sizeof(t_update_object_speed)
	*/

	typedef struct	s_update_object_speed
	{
		size_t		object_id;
		glm::vec2	speed;
	}				t_update_object_speed;

	/*
	**	TYPE_UPDATE_OBJECT_SCALE
	**
	**	header.size = sizeof(t_header) + sizeof(t_update_object_scale)
	*/

	typedef struct	s_update_object_scale
	{
		size_t		object_id;
		glm::vec2	scale;
	}				t_update_object_scale;

	/*
	**	TYPE_UPDATE_OBJECT_ANGLE
	**
	**	header.size = sizeof(t_header) + sizeof(t_update_object_angle)
	*/

	typedef struct	s_update_object_angle
	{
		size_t		object_id;
		double		angle;
	}				t_update_object_angle;

	/*
	**	TYPE_UPDATE_OBJECT_ROT_SPEED
	**
	**	header.size = sizeof(t_header) + sizeof(t_update_object_rot_speed)
	*/

	typedef struct	s_update_object_rot_speed
	{
		size_t		object_id;
		double		speed;
	}				t_update_object_rot_speed;

	/*
	**	TYPE_GLOBAL_MESSAGE
	**
	**	header.size = sizeof(t_header) + sizeof(t_global_message) + global_message.length
	*/

	typedef struct	s_global_message
	{
		size_t		source_id;
		size_t		length;
	}				t_global_message;

	/*
	**	TYPE_PRIVATE_MESSAGE
	**
	**	header.size = sizeof(t_header) + sizeof(t_private_message) + private_message.length
	*/

	typedef struct	s_private_message
	{
		size_t		source_id;
		size_t		destinary_id;
		size_t		length;
	}				t_private_message;

	/*
	**	TYPE_OBJECT_SAY
	**
	**	header.size = sizeof(t_header) + sizeof(t_object_say) + object_say.length
	*/

	typedef struct	s_object_say
	{
		size_t		object_id;
		float		intensity;
		size_t		length;
	}				t_object_say;

	/*
	**	TYPE_OBJECT_SET_SPRITE
	**
	**	header.size = sizeof(t_header) + sizeof(t_object_set_sprite)
	*/

	typedef struct	s_object_set_sprite
	{
		size_t		object_id;
		size_t		sprite;
	}				t_object_set_sprite;

};
