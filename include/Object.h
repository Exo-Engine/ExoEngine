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

#include <glm/vec2.hpp>
#include <mutex>
#include "hitboxes.h"
#include "sprite.h"

namespace	ExoEngine
{

class	World;

class	Object
{
	public:
		typedef enum
		{
			BLOCK,
			ENTITY
		}	objectType;

		Object(size_t id, const objectType &type, uint32_t bitfield, const glm::vec2 &pos, const glm::vec2 scale, const glm::vec2 &speed, double angle, double rotSpeed, std::shared_ptr<hitboxes> hitboxes, size_t resourceId);
		virtual ~Object(void);

		const glm::vec2	&getPos(void) const;

		virtual void	setPos(const glm::vec2 &pos);
		virtual void	translate(const glm::vec2 &vector);

		const glm::vec2	&getScale(void) const;

		void	setScale(const glm::vec2 &scale);
		void	rescale(const glm::vec2 &factor);

		const glm::vec2	&getSpeed(void) const;

		void	setSpeed(const glm::vec2 &speed);
		void	accelerate(const glm::vec2 &acceleration);

		const double	&getAngle(void) const;

		void	setAngle(const double &angle);
		void	rotate(const double &angle);

		const double	&getRotationSpeed(void) const;

		void	setRotationSpeed(const double &speed);
		void	accelerateRotation(const double &acceleration);

		void		setField(uint32_t field);
		bool		getField(uint32_t field) const;
		void		removeField(uint32_t field);

		size_t							getId(void) const;
		const objectType				&getType(void) const;
		const std::shared_ptr<hitboxes>	&getHitboxes(void) const;
		ExoRenderer::sprite				*getSprite(void);

		bool			collide(const glm::vec2 &pos) const;

		void			handlePhysic(const float &elapsedTime);
		virtual void	handleMovement(const float &elapsedTime);
		void			handleRotation(const float &elapsedTime);

		virtual void	handleMovement(const float &elapsedTime, const glm::vec2 &acceleration);
		void			handleRotation(const float &elapsedTime, const float &acceleration);

		virtual void	handleMovementAccelerate(const float &elapsedTime, const glm::vec2 &acceleration);
		void			handleRotationAccelerate(const float &elapsedTime, const float &acceleration);

		float			distance(const glm::vec2 &pos);

		size_t					getLayer(void) const;

		size_t					getResourceId(void) const;
	private:
		size_t						_id;
		objectType					_type;
		uint32_t					_bitfield;
		glm::vec2					_pos;
		glm::vec2					_scale;
		glm::vec2					_speed;
		double						_angle;
		double						_rotSpeed;
		std::shared_ptr<hitboxes>	_hitboxes;
		ExoRenderer::sprite			_sprite;
		size_t						_resourceId;
};

}
