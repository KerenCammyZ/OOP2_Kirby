#pragma once

#include "GameObject.h"

class MovingObject : public GameObject
{
public:
	MovingObject();
	virtual ~MovingObject();

	virtual void update(float deltaTime) override;
	virtual void move(float deltaTime) = 0;
	virtual void initPhysics(b2World& world, const b2Vec2& sizeMeters, const b2Vec2& positionMeters);

	void setVelocity(const b2Vec2& velocity);
	b2Vec2 getVelocity() const;

protected:
	float m_speed = 0.1f; // Default speed
	bool fixedRotation = true;
	float density = 1.0f;
	float friction = 0.3f;
};