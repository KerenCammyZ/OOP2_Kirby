// Floor.cpp
#include "GameObj/FixedObj/Floor.h"
#include "GameObj/MovingObj/Kirby.h"
#include <iostream>
#include "GameObjectFactory.h"
#include "GlobalSizes.h"

const sf::Color floorColor(76, 255, 0);

bool Floor::m_registeritem = GameObjectFactory::registerType(
	floorColor, // The color key for a Floor object
	[](sf::Vector2f position, Kirby* kirby) -> std::unique_ptr<GameObject> // The blueprint
	{
		// This lambda function creates and returns a new Floor object
		// when the factory is asked to create one for the color green.
		auto floor = std::make_unique<Floor>();
		// Note: position is passed to the factory create function but not used here
		// because Floor's constructor doesn't need it. The position is set later.
		return floor;
	}
);

//void Floor::handleCollision(Kirby* kirby)
//{
//	sf::FloatRect kirbyBounds = kirby->getBounds();
//	sf::FloatRect floorBounds = getBounds();
//	sf::Vector2f kirbyPrevPos = kirby->getOldPosition();
//	const int floorHeight = floorBounds.height;
//	
//	// Check if Kirby was coming from above in the previous frame
//	if (kirbyPrevPos.y + kirbyBounds.height / 2.f <= floorBounds.top)
//	{
//		// 1. Stop his vertical movement completely.
//		kirby->setVelocity({ kirby->getVelocity().x, 0.f });
//
//		// 2. Mark Kirby as being on the ground.
//		kirby->setGrounded(true);
//
//		// 3. Reposition him to be exactly on top of the floor.
//		kirby->setPosition({ kirby->getPosition().x, floorBounds.top - (kirbyBounds.height / 2.f) });
//	}
//
//	else if (kirbyPrevPos.x + kirbyBounds.width / 2.f <= floorBounds.left) // Coming from the left
//	{
//		// Stop horizontal movement and reposition Kirby to the left and on top of the floor
//		kirby->setVelocity({ 0.f, kirby->getVelocity().y });
//		kirby->setPosition({ floorBounds.left - (kirbyBounds.width / 2.f), kirby->getPosition().y - floorHeight });
//	}
//	else if (kirbyPrevPos.x - kirbyBounds.width / 2.f >= floorBounds.left + floorBounds.width) // Coming from the right
//	{
//		// Stop horizontal movement and reposition Kirby to the right and on top of the floor
//		kirby->setVelocity({ 0.f, kirby->getVelocity().y });
//		kirby->setPosition({ floorBounds.left + floorBounds.width + (kirbyBounds.width / 2.f), kirby->getPosition().y - floorHeight });
//	}
// 
//}
//void Floor::handleCollision(Kirby* kirby)
//{
//	sf::FloatRect kirbyBounds = kirby->getBounds();
//	sf::FloatRect floorBounds = getBounds();
//	sf::Vector2f kirbyPrevPos = kirby->getOldPosition();
//	const int floorHeight = floorBounds.height;
//	
//	// Check if Kirby was coming from above OR is already walking on the floor.
//	// A small tolerance (e.g., 1.0f) handles floating-point inaccuracies.
//	if (kirbyPrevPos.y + kirbyBounds.height / 2.f <= floorBounds.top + 1.0f)
//	{
//		// 1. Stop his vertical movement completely.
//		kirby->setVelocity({ kirby->getVelocity().x, 0.f });
//
//		// 2. Mark Kirby as being on the ground.
//		kirby->setGrounded(true);
//
//		// 3. Reposition him to be exactly on top of the floor.
//		kirby->setPosition({ kirby->getPosition().x, floorBounds.top - (kirbyBounds.height / 2.f) });
//	}
//
//	else if (kirbyPrevPos.x + kirbyBounds.width / 2.f <= floorBounds.left) // Coming from the left
//	{
//		// Stop horizontal movement and reposition Kirby to the left and on top of the floor
//		kirby->setVelocity({ 0.f, kirby->getVelocity().y });
//		kirby->setPosition({ floorBounds.left - (kirbyBounds.width / 2.f), kirby->getPosition().y - floorHeight });
//	}
//	else if (kirbyPrevPos.x - kirbyBounds.width / 2.f >= floorBounds.left + floorBounds.width) // Coming from the right
//	{
//		// Stop horizontal movement and reposition Kirby to the right and on top of the floor
//		kirby->setVelocity({ 0.f, kirby->getVelocity().y });
//		kirby->setPosition({ floorBounds.left + floorBounds.width + (kirbyBounds.width / 2.f), kirby->getPosition().y - floorHeight });
//	}
//}

// In Floor.cpp, REPLACE your handleCollision function with this one.

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

		// Reliably set his status to grounded.
		/*kirby->setGrounded(true);*/
	}
}

void Floor::handleCollision(Enemy* enemy)
{
	// This simple check is enough for hopping/patrolling enemies.
	// It tells the enemy that it is on the ground.
	enemy->setGrounded(true);

	//// For hopping enemies like Sparky, we can also stop their downward movement.
	//if (auto hoppingMove = dynamic_cast<HoppingMove*>(enemy->getMoveBehavior())) {
	//	// This is a more advanced step, but it would prevent the hop from
	//	// going "through" the floor. For now, just setting the flag is enough.
	//}
}