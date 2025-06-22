#include "States/KirbyMovingState.h"
#include "States/KirbyStandingState.h" // To transition back to standing
#include "GameObj/MovingObj/Kirby.h"   // To call Kirby's public methods
#include <SFML/Window/Keyboard.hpp>

std::unique_ptr<KirbyState> KirbyMovingState::handleInput()
{
	// If no movement keys are pressed, transition back to the Standing state.
	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Left) &&
		!sf::Keyboard::isKeyPressed(sf::Keyboard::Right) &&
		!sf::Keyboard::isKeyPressed(sf::Keyboard::Up) &&
		!sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		return std::make_unique<KirbyStandingState>();
	}

	// Otherwise, remain in the Moving state.
	return nullptr;
}

void KirbyMovingState::update(Kirby& kirby, float deltaTime)
{
	sf::Vector2f movement(0.f, 0.f);
	float speed = kirby.getSpeed(); // Get Kirby's speed

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		movement.x -= speed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		movement.x += speed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		movement.y -= speed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		movement.y += speed;
	}

	// Update Kirby's position using its public setPosition method.
	kirby.setPosition(kirby.getPosition() + movement * deltaTime);
}