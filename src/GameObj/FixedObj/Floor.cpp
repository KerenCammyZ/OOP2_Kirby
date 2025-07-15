// Floor.cpp
#include "GameObj/FixedObj/Floor.h"
#include "GameObj/MovingObj/Kirby.h"
#include <iostream>
#include "GameObjectFactory.h"
#include "GlobalSizes.h"

const sf::Color floorColor(76, 255, 0);

bool Floor::m_registeritem = GameObjectFactory::registerType(
	floorColor,
	[](sf::Vector2f position, Kirby* kirby) -> std::unique_ptr<GameObject> // The blueprint
	{
		auto floor = std::make_unique<Floor>();
		return floor;
	}
);

void Floor::handleCollision(Kirby* kirby)
{
	// If Kirby is moving downwards or horizontally (velocity >= 0), he should land.
	// This simple condition works for both landing from a jump and walking horizontally.
	if (kirby->getVelocity().y >= 0)
	{
		sf::FloatRect kirbyBounds = kirby->getBounds();
		sf::FloatRect floorBounds = getBounds();

		// Snap Kirby's position to be exactly on top of the floor.
		kirby->setPosition({ kirby->getPosition().x, floorBounds.top - (kirbyBounds.height / 2.f) });

		// Stop his vertical movement.
		kirby->setVelocity({ kirby->getVelocity().x, 0.f });
	}
}

void Floor::handleCollision(Enemy* enemy)
{
}