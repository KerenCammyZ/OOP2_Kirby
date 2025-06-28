// Floor.cpp
#include "GameObj/FixedObj/Floor.h"
#include "GameObj/MovingObj/Kirby.h"
#include <iostream>
#include "GameObjectFactory.h"
#include "GlobalSizes.h"

const sf::Color floorColor(76, 255, 0);

bool Floor::m_registeritem = GameObjectFactory::registerType(
	floorColor, // The color key for a Floor object
	[](sf::Vector2f position) -> std::unique_ptr<GameObject> // The blueprint
	{
		// This lambda function creates and returns a new Floor object
		// when the factory is asked to create one for the color green.
		auto floor = std::make_unique<Floor>();
		// Note: position is passed to the factory create function but not used here
		// because Floor's constructor doesn't need it. The position is set later.
		return floor;
	}
);

void Floor::handleCollision(Kirby* kirby)
{
	sf::FloatRect kirbyBounds = kirby->getBounds();
	sf::FloatRect floorBounds = getBounds();
	sf::Vector2f kirbyPrevPos = kirby->getOldPosition();

	// Check if Kirby was coming from above in the previous frame
	if (kirbyPrevPos.y + kirbyBounds.height / 2.f <= floorBounds.top)
	{
		// 1. Stop his vertical movement completely.
		kirby->setVelocity({ kirby->getVelocity().x, 0.f });

		// 2. Mark Kirby as being on the ground.
		kirby->setGrounded(true);

		// 3. Reposition him to be exactly on top of the floor.
		kirby->setPosition({ kirby->getPosition().x, floorBounds.top - (kirbyBounds.height / 2.f) });
	}
}