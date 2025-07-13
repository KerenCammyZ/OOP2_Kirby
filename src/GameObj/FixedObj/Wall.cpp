// Wall.cpp
#include "GameObj/FixedObj/Wall.h"
#include "GameObj/MovingObj/Kirby.h"
#include "GameObjectFactory.h"
#include "GlobalSizes.h"
#include <iostream>

sf::Color wallColor(255, 0, 0);

bool Wall::m_registeritem = GameObjectFactory::registerType(
	sf::Color(wallColor), // The color key for a Wall object
	[](sf::Vector2f position, Kirby* kirby) -> std::unique_ptr<GameObject>
	{
		auto wall = std::make_unique<Wall>();
		return wall;
	}
);

void Wall::handleCollision(Kirby* kirby)
{
	;
}

void Wall::draw(sf::RenderTarget& target) const
{
	;
}

void Wall::setSize(const sf::Vector2f& size)
{
	GameObject::setSize(size);
	m_collisionShape.setSize(size);
}

void Wall::setPosition(const sf::Vector2f& position)
{
	GameObject::setPosition(position);
	m_collisionShape.setPosition(position);
}