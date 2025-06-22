// Wall.cpp
#include "GameObj/FixedObj/Wall.h"
#include "GameObj/MovingObj/Kirby.h"

void Wall::handleCollision(Kirby* kirby)
{
	kirby->setPosition(kirby->getOldPosition());
}

void Wall::draw(sf::RenderTarget& target) const
{
	;
}

void Wall::setSize(const sf::Vector2f& size)
{
	GameObject::setSize(size);
	//m_debugShape.setSize(size);
	//m_debugShape.setOrigin(size.x / 2.f, size.y / 2.f); // Keep origin centered
}

void Wall::setPosition(const sf::Vector2f& position)
{
	GameObject::setPosition(position);
	//m_debugShape.setPosition(position);
}
