#pragma once

#include "GameObject.h"

class MovingObject : public GameObject
{
public:
	MovingObject();
	virtual ~MovingObject() = default;

	virtual void update(float deltaTime) override;

	void initPhysics(b2World& world, const b2Vec2& sizeMeters, const b2Vec2& positionMeters,
					bool fixedRotation = true, float density = 1.0f, float friction = 0.3f);

	void setVelocity(const b2Vec2& velocity);
	b2Vec2 getVelocity() const;
};