#include "Kirby.h"

Kirby::Kirby() {};

void Kirby::move(float deltaTime)
{

	sf::Vector2f movement(0.f, 0.f);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		movement.x -= m_speed;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		movement.x += m_speed;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		movement.y -= m_speed;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		movement.y += m_speed;
	}

	// Update the object's logical position
	m_position += movement * deltaTime;
}