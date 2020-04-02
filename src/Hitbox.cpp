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

#include "Hitbox.h"

using namespace	ExoEngine;

Hitbox::Hitbox(const glm::vec2 &absolute, const hitbox &src, const float &weight) : _pos(src.x, src.y), _size(src.w, src.h)
{
	_shape = new btBoxShape(btVector3(_size.x, _size.y, 1));
	_transform.setIdentity();
	_transform.setOrigin(btVector3(_pos.x + absolute.x, _pos.y + absolute.y, 0));
	_motionState = new btDefaultMotionState(_transform);
	_body = new btRigidBody(btRigidBody::btRigidBodyConstructionInfo(weight, _motionState, _shape));
}

Hitbox::~Hitbox(void)
{
	_shape;
	_motionState;
	_body;
}

const glm::vec2&	Hitbox::getPos(void)
{
	return (_pos);
}

btRigidBody*	Hitbox::getBody(void) const
{
	return (_body);
}

glm::mat4		Hitbox::getTransform(void)
{
	btScalar	buffer[16];
	glm::mat4	matrix(1);

	_motionState->m_graphicsWorldTrans.getOpenGLMatrix((btScalar *)&buffer);
	for (size_t x = 0; x < 4; x++)
		for (size_t y = 0; y < 4; y++)
			matrix[y][x] = buffer[y * 4 + x];
	return (matrix);
}
