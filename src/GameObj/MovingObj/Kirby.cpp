//// Kirby.cpp
//#include "GameObj/MovingObj/Kirby.h"
//#include "GameObj/FixedObj/Door.h"
//
//Kirby::Kirby(std::shared_ptr<sf::Texture>& kirbyTexture)
//{
//	setTexture(kirbyTexture);
//	sf::Vector2f kirbySize(ENTITY_SIZE, ENTITY_SIZE);
//	setSize(kirbySize);
//	sf::Vector2f startPosition(50,50);
//	setPosition(startPosition);
//};
//
//void Kirby::handleCollision(Door* door)
//{
//	door->handleCollision(this);
//}
//
//void Kirby::move(float deltaTime)
//{
//
//	sf::Vector2f movement(0.f, 0.f);
//
//	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
//	{
//		movement.x -= m_speed;
//	}
//	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
//	{
//		movement.x += m_speed;
//	}
//
//	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
//	{
//		movement.y -= m_speed;
//	}
//	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
//	{
//		movement.y += m_speed;
//	}
//
//	// Update the object's logical position
//	m_position += movement * deltaTime;
//}
//
//void Kirby::handleCollision(GameObject* other)
//{
//	other->handleCollision(this);
//}

// Kirby.cpp
#include "GameObj/MovingObj/Kirby.h"
#include "GameObj/FixedObj/Door.h"
#include "States/KirbyStandingState.h" // Include the initial state

Kirby::Kirby(std::shared_ptr<sf::Texture>& kirbyTexture)
{
	setTexture(kirbyTexture);
	sf::Vector2f kirbySize(ENTITY_SIZE, ENTITY_SIZE);
	setSize(kirbySize);
	sf::Vector2f startPosition(50, 50);
	setPosition(startPosition);

	// Set the initial state to StandingState
	m_state = std::make_unique<KirbyStandingState>();
	m_state->enter(*this);
};

// Add the definition for the speed getter
float Kirby::getSpeed() const
{
	return m_speed;
}

void Kirby::handleCollision(Door* door)
{
	door->handleCollision(this);
}

// The move method is now completely driven by the state pattern.
// This single function replaces all the previous input-handling logic.
void Kirby::move(float deltaTime)
{
	// 1. Let the current state check for input that causes a state change.
	auto newState = m_state->handleInput();

	// 2. If a new state is returned, transition to it.
	if (newState)
	{
		m_state = std::move(newState);
		m_state->enter(*this); // Call the entry action for the new state
	}

	// 3. Update Kirby's behavior based on the current state.
	m_state->update(*this, deltaTime);
}

void Kirby::handleCollision(GameObject* other)
{
	other->handleCollision(this);
}
