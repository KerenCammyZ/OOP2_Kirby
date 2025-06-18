// Kirby.cpp
#include "GameObj/MovingObj/Kirby.h"
#include "GameObj/FixedObj/Door.h"

Kirby::Kirby(std::shared_ptr<sf::Texture>& kirbyTexture)
{
	setTexture(kirbyTexture);
	sf::Vector2f kirbySize(ENTITY_SIZE, ENTITY_SIZE);
	setSize(kirbySize);
	sf::Vector2f startPosition(50,50);
	setPosition(startPosition);
};

void Kirby::handleCollision(Door* door)
{
	door->handleCollision(this);
}

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

void Kirby::handleCollision(GameObject* other)
{
	other->handleCollision(this);
}
