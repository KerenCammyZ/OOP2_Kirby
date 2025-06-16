#include "Floor.h"
#include "Kirby.h"
#include <iostream>
#include "Wall.h"

void Wall::handleCollision(Kirby* kirby)
{
	// Get the bounding boxes of both objects for collision resolution
	sf::FloatRect kirbyBounds = kirby->getBounds();
	sf::FloatRect wallBounds = getBounds();

	// Get Kirby's position from the previous frame to determine the direction of approach
	sf::Vector2f kirbyPrevPos = kirby->getOldPosition();
	sf::FloatRect kirbyPrevBounds(kirbyPrevPos - sf::Vector2f(kirbyBounds.width, kirbyBounds.height), kirbyBounds.getSize());

	// A small tolerance to prevent floating-point inaccuracies and ensure
	// the collision registers correctly when objects are flush.
	float collisionTolerance = 5.f;


	if (kirbyPrevBounds.top + kirbyPrevBounds.height <= wallBounds.top + collisionTolerance)
	{
		kirby->setPosition({ kirby->getPosition().x, wallBounds.top - (kirbyBounds.height / 2.f) });
	}
	else if (kirbyPrevBounds.left <= wallBounds.left + collisionTolerance)
	{
		// If Kirby is coming from the left, we can push him to the right
		kirby->setPosition({ wallBounds.left + (kirbyBounds.width / 2.f), kirby->getPosition().y });
	}
	else if (kirbyPrevBounds.left >= wallBounds.left + wallBounds.width - collisionTolerance)
	{
		// If Kirby is coming from the right, we can push him to the left
		kirby->setPosition({ wallBounds.left + wallBounds.width - (kirbyBounds.width / 2.f), kirby->getPosition().y });
	}
	else if (kirbyPrevBounds.top >= wallBounds.top + wallBounds.height - collisionTolerance)
	{
		// If Kirby is coming from below, we can push him up
		kirby->setPosition({ kirby->getPosition().x, wallBounds.top + wallBounds.height + (kirbyBounds.height / 2.f)});
	}
	else
	{
		std::cout << "Unhandled collision case with Wall.\n";
	}

}
