// MovingObject.cpp
#include "GameObj/MovingObj/MovingObject.h"

MovingObject::MovingObject() {}

MovingObject::~MovingObject() = default;


void MovingObject::update(float dt)
{
	m_oldPosition = m_position;
	move(dt);
	GameObject::update(dt);
}

void MovingObject::setOldPosition(const sf::Vector2f& position)  
{  
    m_oldPosition = position;  
}


sf::Vector2f MovingObject::getOldPosition() const
{
	return m_oldPosition;
}

sf::Vector2f MovingObject::getDirection() const
{
	return m_direction;
}

void MovingObject::setDirection(const sf::Vector2f& direction)
{
	m_direction = direction;
}
