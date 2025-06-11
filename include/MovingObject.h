#pragma once

#include "GameObject.h"

class MovingObject : public GameObject
{
public:
	MovingObject();
	virtual ~MovingObject();

	virtual void update(float deltaTime) override;
	virtual void move(float deltaTime) = 0;

protected:
	float m_speed = 0.1f; // Default speed
};