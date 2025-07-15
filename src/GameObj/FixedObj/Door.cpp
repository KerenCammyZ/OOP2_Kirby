#include "GameObj/FixedObj/Door.h"
#include "GameObj/MovingObj/Kirby.h"
#include <thread>
#include <chrono>

Door::Door(sf::Vector2f destination) : m_destination(destination)
{
	m_collisionShape.setFillColor(sf::Color(255, 0, 255, 100)); // Semi-transparent magenta
}

void Door::handleCollision(Kirby* kirby)
{
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		sf::Vector2f arrivalPosition = m_destination;
		arrivalPosition.x += kirby->getSize().x;
		kirby->setPosition(arrivalPosition);
	}
}