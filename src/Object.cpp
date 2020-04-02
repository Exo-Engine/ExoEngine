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

#include "Object.h"
#include <glm/mat2x2.hpp>
#include <glm/ext.hpp>
#include <math.h>

using namespace	ExoEngine;
using namespace	ExoRenderer;

Object::Object(size_t id, const objectType &type, uint32_t bitfield, const glm::vec2 &pos, const glm::vec2 scale, const glm::vec2 &speed, double angle, double rotSpeed, std::shared_ptr<hitboxes> hitboxes, size_t resourceId) :
	_id(id), _type(type), _bitfield(bitfield), _pos(pos), _scale(scale), _speed(speed), _angle(angle), _rotSpeed(rotSpeed), _hitboxes(hitboxes), _sprite(), _resourceId(resourceId)
{
	_sprite = sprite();
}

Object::~Object(void)
{
}

const glm::vec2	&Object::getPos(void) const
{
	return (_pos);
}

void	Object::setPos(const glm::vec2 &pos)
{
	_pos = pos;
}

void	Object::translate(const glm::vec2 &vector)
{
	_pos += vector;
}

const glm::vec2	&Object::getScale(void) const
{
	return (_scale);
}

void	Object::setScale(const glm::vec2 &scale)
{
	_scale = scale;
}

void	Object::rescale(const glm::vec2 &factor)
{
	_scale *= factor;
}

const glm::vec2	&Object::getSpeed(void) const
{
	return (_speed);
}

void	Object::setSpeed(const glm::vec2 &speed)
{
	_speed = speed;
}

void	Object::accelerate(const glm::vec2 &acceleration)
{
	_speed += acceleration;
}

const double	&Object::getAngle(void) const
{
	return (_angle);
}

void	Object::setAngle(const double &angle)
{
	_angle = fmod(angle, glm::pi<double>());
}

void	Object::rotate(const double &angle)
{
	_angle = fmod((_angle + angle), glm::pi<double>());
}

const double	&Object::getRotationSpeed(void) const
{
	return (_rotSpeed);
}

void			Object::setRotationSpeed(const double &speed)
{
	_rotSpeed = speed;
}

void			Object::accelerateRotation(const double &acceleration)
{
	_rotSpeed *= acceleration;
}

size_t Object::getId(void) const
{
	return (_id);
}

void		Object::setField(uint32_t field)
{
	_bitfield |= field;
}

bool		Object::getField(uint32_t field) const
{
	return ((_bitfield & field));
}

void		Object::removeField(uint32_t field)
{
	_bitfield = (_bitfield & (~field));
}

const Object::objectType &Object::getType(void) const
{
	return (_type);
}

const std::shared_ptr<hitboxes> &Object::getHitboxes(void) const
{
	return (_hitboxes);
}

sprite *Object::getSprite(void)
{
	return (&_sprite);
}

bool	Object::collide(const glm::vec2 &pos) const
{
	double		c = cos(-_angle);
	double		s = sin(-_angle);
	glm::vec2	tmp = glm::mat2(c, -s, s, c) * pos;

	if (tmp.x >= _pos.x && tmp.x < _pos.x + _scale.x &&
		tmp.y >= _pos.y && tmp.y < _pos.y + _scale.y)
		return (true);
	return (false);
}

void	Object::handlePhysic(const float &elapsedTime)
{
	handleMovement(elapsedTime);
	handleRotation(elapsedTime);
}

void	Object::handleMovement(const float &elapsedTime)
{
	_pos = _speed * elapsedTime + _pos;
}

void	Object::handleRotation(const float &elapsedTime)
{
	_angle = _rotSpeed * elapsedTime + _angle;
}

void	Object::handleMovement(const float &elapsedTime, const glm::vec2 &acceleration)
{
	_pos = ((acceleration * elapsedTime * elapsedTime) / (float)2) + _speed * elapsedTime + _pos;
	_speed += (acceleration * elapsedTime);
}

void	Object::handleRotation(const float &elapsedTime, const float &acceleration)
{
	_angle = ((acceleration * elapsedTime * elapsedTime) / (float)2) + _rotSpeed * elapsedTime + _angle;
	_rotSpeed += (acceleration * elapsedTime);
}

void	Object::handleMovementAccelerate(const float &elapsedTime, const glm::vec2 &acceleration)
{
	glm::vec2				speed(_speed);
	glm::vec2				expo = glm::vec2(pow(acceleration.x, elapsedTime), pow(acceleration.y, elapsedTime));

	_speed = _speed * expo;
	if (acceleration.x)
		_pos.x = (speed.x * (expo.x - 1)) / log(acceleration.x) + _pos.x;
	if (acceleration.y)
		_pos.y = (speed.y * (expo.y - 1)) / log(acceleration.y) + _pos.y;
}

void	Object::handleRotationAccelerate(const float &elapsedTime, const float &acceleration)
{
	double	speed(_rotSpeed);
	double	expo = pow(acceleration, elapsedTime);

	_rotSpeed = _rotSpeed * expo;
	if (acceleration)
		_angle = (speed * (expo - 1)) / log(acceleration) + _angle;
}

float	Object::distance(const glm::vec2 &pos)
{
	return (sqrt(pow(pos.x - _pos.x, 2) + pow(pos.y - _pos.y, 2)));
}

size_t					Object::getLayer(void) const
{
#ifdef CLIENT
	return (_sprite.layer);
#else
	return (0);
#endif
}

size_t					Object::getResourceId(void) const
{
	return (_resourceId);
}
