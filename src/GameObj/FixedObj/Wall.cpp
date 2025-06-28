// Wall.cpp
#include "GameObj/FixedObj/Wall.h"
#include "GameObj/MovingObj/Kirby.h"
#include "GameObjectFactory.h"
#include "GlobalSizes.h"
#include <iostream>

sf::Color wallColor(255, 0, 0);

bool Wall::m_registeritem = GameObjectFactory::registerType(
	sf::Color(wallColor), // The color key for a Wall object
	[](sf::Vector2f position) -> std::unique_ptr<GameObject> // The blueprint
	{
		// This lambda function creates and returns a new wall object
		// when the factory is asked to create one for the color red.
		auto wall = std::make_unique<Wall>();
		// Note: position is passed to the factory create function but not used here
		// because Wall's constructor doesn't need it. The position is set later.
		return wall;
	}
);

void Wall::handleCollision(Kirby* kirby)
{
	//kirby->setPosition(kirby->getOldPosition());
	kirby->handleCollision(this);
}

void Wall::draw(sf::RenderTarget& target) const
{
	;
}

void Wall::setSize(const sf::Vector2f& size)
{
	GameObject::setSize(size);
	m_collisionShape.setSize(size);
	//m_collisionShape.setFillColor(sf::Color::Red); // Make it invisible
	//m_collisionShape.setOrigin(size.x / 2.f, size.y / 2.f);
	//m_debugShape.setSize(size);
	//m_debugShape.setOrigin(size.x / 2.f, size.y / 2.f); // Keep origin centered
}

void Wall::setPosition(const sf::Vector2f& position)
{
	GameObject::setPosition(position);
	m_collisionShape.setPosition(position);
	//m_debugShape.setPosition(position);
}

// Implement the override for the pure virtual function
//void Wall::handleCollision(Door* door)
//{
//	 A wall doesn't interact with a door, so this is empty.
//}