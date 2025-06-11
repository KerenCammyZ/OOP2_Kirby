#pragma once

#include "GameObject.h"

class MovingObject : public GameObject
{
public:
	MovingObject();
	virtual ~MovingObject();

	virtual void update(float deltaTime) override;
	virtual void move(float deltaTime) = 0;

	void setOldPosition(const sf::Vector2f& position);
	sf::Vector2f getOldPosition() const;

protected:
	float m_speed = 200.0f;
	sf::Vector2f m_oldPosition;
};