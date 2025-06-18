// Floor.cpp
#include "GameObj/FixedObj/Floor.h"
#include "GameObj/MovingObj/Kirby.h"
#include <iostream>


void Floor::handleCollision(Kirby* kirby)
{
	// Get the bounding boxes of both objects for collision resolution
	sf::FloatRect kirbyBounds = kirby->getBounds();
	sf::FloatRect floorBounds = getBounds();
	
	
	// Get Kirby's position from the previous frame to determine the direction of approach
	sf::Vector2f kirbyPrevPos = kirby->getOldPosition();
	sf::FloatRect kirbyPrevBounds(kirbyPrevPos - sf::Vector2f(kirbyBounds.width / 2.f, kirbyBounds.height / 2.f), kirbyBounds.getSize());

	// A small tolerance to prevent floating-point inaccuracies and ensure
	// the collision registers correctly when objects are flush.
	float collisionTolerance = 5.0f;

	// LOGIC: Check if the bottom of Kirby in the PREVIOUS frame was above the top of the floor.
	if (kirbyPrevBounds.top + kirbyPrevBounds.height <= floorBounds.top + collisionTolerance)
	{
		// Resolve the collision by repositioning Kirby to sit exactly on top of the floor.
		kirby->setPosition({ kirby->getPosition().x, floorBounds.top - (kirbyBounds.height / 2.f) });

		// FUTURE: If you add gravity and jumping, you would also want to tell Kirby
		// that he is grounded and reset his vertical velocity here.
		// For example: kirby.setGrounded(true);
	}
}