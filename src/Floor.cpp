#include "Floor.h"
#include "Kirby.h"
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
	float collisionTolerance = 5.f;

	// LOGIC: Check if the bottom of Kirby in the PREVIOUS frame was above the top of the floor.
	if (kirbyPrevBounds.top + kirbyPrevBounds.height <= floorBounds.top + collisionTolerance)
	{
		// If true, this is a valid top-down collision.
		std::cout << "Kirby landed on Floor from above.\n";

		// Resolve the collision by repositioning Kirby to sit exactly on top of the floor.
		// This prevents him from sinking into it.
		kirby->setPosition({ kirby->getPosition().x, floorBounds.top - kirbyBounds.height });

		// FUTURE: If you add gravity and jumping, you would also want to tell Kirby
		// that he is grounded and reset his vertical velocity here.
		// For example: kirby.setGrounded(true);
	}
	// If the condition is false, it means Kirby is coming from the side or,
	// more importantly, from below. In that case, we do nothing,
	// allowing him to pass through the floor.
}