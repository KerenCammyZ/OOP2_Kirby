#include "GameObj/FixedObj/Door.h"
#include "GameObj/MovingObj/Kirby.h"

// Store the destination when the door is created
Door::Door(sf::Vector2f destination) : m_destination(destination)
{
	// You can set a debug color for doors to see them
	m_debugShape.setFillColor(sf::Color(255, 0, 255, 100)); // Semi-transparent magenta
}

void Door::handleCollision(Kirby* kirby)
{
	// To prevent instantly teleporting back and forth, add a small offset
	// to the destination, pushing Kirby out of the exit door slightly.
	// This assumes the player enters from the bottom of the door.
	sf::Vector2f arrivalPosition = m_destination;
	arrivalPosition.y += kirby->getSize().y; // Adjust based on Kirby's size

	kirby->setPosition(arrivalPosition);

	// Future improvement: Instead of teleporting instantly, you could
	// add a check here for `sf::Keyboard::isKeyPressed(sf::Keyboard::Up)`
	// so the player has to choose to enter the door.
}