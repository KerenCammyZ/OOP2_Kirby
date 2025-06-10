#pragma once

#include "GameObject.h"

class MovingObject : public GameObject
{
public:
	MovingObject();
	virtual ~MovingObject();

	virtual void update(float deltaTime) override;
	virtual void move(float deltaTime) = 0;

	void initPhysics(b2World& world, const b2Vec2& positionMeters, const b2Vec2& sizeMeters = b2Vec2(0.01f, 0.01f),
					bool fixedRotation = true, float density = 1.0f, float friction = 0.3f);

	void setVelocity(const b2Vec2& velocity);
	b2Vec2 getVelocity() const;

protected:
	float m_speed = 0.1f; // Default speed
};