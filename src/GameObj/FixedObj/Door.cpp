#include "GameObj/FixedObj/Door.h"
#include "GameObj/MovingObj/Kirby.h"

Door::Door(sf::Vector2f destination) : m_destination(destination)
{
	m_debugShape.setFillColor(sf::Color(255, 0, 255, 100)); // Semi-transparent magenta
}

void Door::handleCollision(Kirby* kirby)
{

	sf::Vector2f arrivalPosition = m_destination;
	arrivalPosition.x += kirby->getSize().x; // Adjust based on Kirby's size

	kirby->setPosition(arrivalPosition);

	// Future improvement: Instead of teleporting instantly, you could
	// add a check here for `sf::Keyboard::isKeyPressed(sf::Keyboard::Up)`
	// so the player has to choose to enter the door.
}