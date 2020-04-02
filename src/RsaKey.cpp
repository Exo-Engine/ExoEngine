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

#include "RsaKey.h"
#include <openssl/bn.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include "Log.h"

using namespace	ExoEngine;

RsaKey::RsaKey(void)
{
	_key = RSA_new();
	if (!_key)
		throw (std::runtime_error(std::string("failed to create rsa key: ").append(ERR_error_string(ERR_get_error(), NULL))));
	regenerate();
}

RsaKey::RsaKey(const std::string publicKey) : _single(true)
{
	BIO		*bio;
	int		ret;

	_key = RSA_new();
	if (!_key)
		throw (std::runtime_error(std::string("failed to create rsa key: ").append(ERR_error_string(ERR_get_error(), NULL))));
	bio = BIO_new(BIO_s_mem());
	if (!bio)
		throw (std::runtime_error(std::string("failed to create new bio: ").append(ERR_error_string(ERR_get_error(), NULL))));
	ret = BIO_write(bio, (const void *)&publicKey[0], publicKey.length());
	if (ret <= 0)
	{
		BIO_free(bio);
		throw (std::runtime_error(std::string("failed to write bio: ").append(ERR_error_string(ERR_get_error(), NULL))));
	}
	if (!PEM_read_bio_RSAPublicKey(bio, &_key, NULL, (void *)""))
	{
		BIO_free(bio);
		throw (std::runtime_error(std::string("failed to create rsa public key from bio: ").append(ERR_error_string(ERR_get_error(), NULL))));
	}
	BIO_free(bio);
}

RsaKey::~RsaKey(void)
{
	RSA_free(_key);
}

void	RsaKey::regenerate(void)
{
	BIGNUM	*e;
	int		ret;
	BIO		*publicKey;
	BIO		*privateKey;

	_log.debug << "generating rsa key" << std::endl;
	e = BN_new();
	if (!e)
		throw (std::runtime_error(std::string("failed to create bignum: ").append(ERR_error_string(ERR_get_error(), NULL))));
	ret = BN_set_word(e, RSA_F4);
	if (!ret)
	{
		BN_free(e);
		throw (std::runtime_error(std::string("failed to assign value to bignum: ").append(ERR_error_string(ERR_get_error(), NULL))));
	}
	ret = RSA_generate_key_ex(_key, 2048, e, NULL);
	if (!ret)
	{
		BN_free(e);
		throw (std::runtime_error(std::string("failed to generate rsa key: ").append(ERR_error_string(ERR_get_error(), NULL))));
	}
	BN_free(e);
	_single = false;

	_log.debug << "rsa key generated" << std::endl
		<< "generating string versions of key pair" << std::endl;
	publicKey = BIO_new(BIO_s_mem());
	if (!publicKey)
		throw (std::runtime_error(std::string("failed to create new bio: ").append(ERR_error_string(ERR_get_error(), NULL))));
	privateKey = BIO_new(BIO_s_mem());
	if (!privateKey)
	{
		BIO_free(publicKey);
		throw (std::runtime_error(std::string("failed to create new bio: ").append(ERR_error_string(ERR_get_error(), NULL))));
	}
	ret = PEM_write_bio_RSAPublicKey(publicKey, _key);
	if (!ret)
	{
		BIO_free(publicKey);
		BIO_free(privateKey);
		throw (std::runtime_error(std::string("failed to write public key in bio: ").append(ERR_error_string(ERR_get_error(), NULL))));
	}
	ret = PEM_write_bio_RSAPrivateKey(privateKey, _key, NULL, NULL, 0, NULL, NULL);
	if (!ret)
	{
		BIO_free(publicKey);
		BIO_free(privateKey);
		throw (std::runtime_error(std::string("failed to write private key in bio: ").append(ERR_error_string(ERR_get_error(), NULL))));
	}
	_publicKey.resize(BIO_pending(publicKey));
	_privateKey.resize(BIO_pending(privateKey));
	ret = BIO_read(publicKey, &_publicKey[0], _publicKey.size());
	if (ret <= 0)
	{
		BIO_free(publicKey);
		BIO_free(privateKey);
		throw (std::runtime_error(std::string("failed to read public key: ").append(ERR_error_string(ERR_get_error(), NULL))));
	}
	ret = BIO_read(privateKey, &_privateKey[0], _privateKey.size());
	if (ret <= 0)
	{
		BIO_free(publicKey);
		BIO_free(privateKey);
		throw (std::runtime_error(std::string("failed to read private key: ").append(ERR_error_string(ERR_get_error(), NULL))));
	}
	BIO_free(publicKey);
	BIO_free(privateKey);
}

Message	RsaKey::encrypt(const Message &message)
{
	Message	dest((size_t)RSA_size(_key));
	int		ret;

	if (message.getSize() >= (size_t)RSA_size(_key) - 41)
		throw (std::runtime_error("message too big (>= RSA_size(rsa) - 41)"));
	ret = RSA_public_encrypt(message.getSize(), (const unsigned char *)message.getPtr(),
		(unsigned char *)dest.getPtr(), _key, RSA_PKCS1_OAEP_PADDING);
	if (ret == -1)
		throw (std::runtime_error(std::string("failed to encrypt message: ").append(ERR_error_string(ERR_get_error(), NULL))));
	dest.resize(ret);
	return (dest);
}

Message	RsaKey::decrypt(const Message &message)
{
	Message	dest((size_t)RSA_size(_key));
	int		ret;

	if (_single)
		throw (std::runtime_error("socket don't have private key, cannot decrypt"));
	ret = RSA_private_decrypt(message.getSize(), (const unsigned char *)message.getPtr(),
		(unsigned char *)dest.getPtr(), _key, RSA_PKCS1_OAEP_PADDING);
	if (ret == -1)
		throw (std::runtime_error(std::string("failed to encrypt message: ").append(ERR_error_string(ERR_get_error(), NULL))));
	dest.resize(ret);
	return (dest);
}

const std::string	&RsaKey::getPublicKey(void)
{
	return (_publicKey);
}

const std::string	&RsaKey::getPrivateKey(void)
{
	return (_privateKey);
}
