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
	//sf::FloatRect kirbyBounds = kirby->getBounds();
	//sf::FloatRect floorBounds = getBounds();
	//sf::Vector2f kirbyPrevPos = kirby->getOldPosition();
	//const int floorHeight = floorBounds.height * 1.5;
	//
	//// Check if Kirby was coming from above OR is already walking on the floor.
	//// A small tolerance (e.g., 1.0f) handles floating-point inaccuracies.
	//if (kirbyPrevPos.y + kirbyBounds.height / 2.f <= floorBounds.top + 1.0f)
	//{
	//	kirby->setVelocity({ kirby->getVelocity().x, 0.f });
	//	kirby->setGrounded(true);
	//	kirby->setPosition({ kirby->getPosition().x, floorBounds.top - (kirbyBounds.height / 2.f) });
	//}
	//else if (kirbyPrevPos.x + kirbyBounds.width / 2.f <= floorBounds.left)
	//{
	//	// Stop horizontal movement and reposition Kirby to the left and on top of the floor
	//	kirby->setVelocity({ 0.f, kirby->getVelocity().y });
	//	kirby->setPosition({ floorBounds.left - (kirbyBounds.width / 2.f), kirby->getPosition().y - floorHeight });
	//}
	//else if (kirbyPrevPos.x - kirbyBounds.width / 2.f >= floorBounds.left + floorBounds.width) 
	//{
	//	// Stop horizontal movement and reposition Kirby to the right and on top of the floor
	//	kirby->setVelocity({ 0.f, kirby->getVelocity().y });
	//	kirby->setPosition({ floorBounds.left + floorBounds.width + (kirbyBounds.width / 2.f), kirby->getPosition().y - floorHeight });
	//}
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

		// Reliably set his status to grounded.
		kirby->setGrounded(true);
	}
}

void Floor::handleCollision(Enemy* enemy)
{
	// This simple check is enough for hopping/patrolling enemies.
	// It tells the enemy that it is on the ground.
	enemy->setGrounded(true);
}