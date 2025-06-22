#include "States/KirbyWalkingState.h" // Renamed
#include "States/KirbyStandingState.h" 
#include "GameObj/MovingObj/Kirby.h"   
#include <SFML/Window/Keyboard.hpp>

std::unique_ptr<KirbyState> KirbyWalkingState::handleInput()
{
	// If LEFT and RIGHT are no longer pressed, go back to standing.
	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Left) &&
		!sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		return std::make_unique<KirbyStandingState>();
	}

	return nullptr;
}

void KirbyWalkingState::update(Kirby& kirby, float deltaTime)
{
	sf::Vector2f movement(0.f, 0.f);
	float speed = kirby.getSpeed();

	// ONLY check for horizontal movement
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		movement.x -= speed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		movement.x += speed;
	}

	// Update Kirby's position. Note we are only changing the X-coordinate.
	kirby.setPosition(kirby.getPosition() + movement * deltaTime);
}