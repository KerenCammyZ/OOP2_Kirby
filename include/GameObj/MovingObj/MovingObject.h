// MovingObject.h
#pragma once
#include "GameObj/GameObject.h"

class MovingObject : public GameObject
{
public:
	MovingObject();
	virtual ~MovingObject();

	virtual void update(float deltaTime) override;
	virtual void move(float deltaTime) = 0;

	void setOldPosition(const sf::Vector2f& position);
	sf::Vector2f getOldPosition() const;

	float getSpeed() const { return m_speed; }
	void setSpeed(float speed) { m_speed = speed; }

	sf::Vector2f getDirection() const;
	void setDirection(const sf::Vector2f& direction);

	void handleCollision(Enemy* enemy) override {};
protected:
	float m_speed = 250.0f;
	sf::Vector2f m_direction;
	sf::Vector2f m_oldPosition;
};