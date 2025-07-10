#include "States/KirbyStates/KirbyWalkingState.h"
#include "States/KirbyStates/KirbyStandingState.h"
#include "States/KirbyStates/KirbyJumpingState.h"
#include "States/KirbyStates/KirbyFallingState.h"  // Include falling
#include "GameObj/MovingObj/Kirby.h"
#include <SFML/Window/Keyboard.hpp>

std::unique_ptr<KirbyState> KirbyWalkingState::handleInput(Kirby& kirby)
{
	// --- THIS IS THE "WALK OFF A CLIFF" LOGIC ---
	// If we are no longer on the ground, we must be falling.
	if (!kirby.isGrounded())
	{
		return std::make_unique<KirbyFallingState>();
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		return std::make_unique<KirbyJumpingState>();
	}

	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Left) &&
		!sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		return std::make_unique<KirbyStandingState>();
	}
	return nullptr;
}

void KirbyWalkingState::update(Kirby& kirby, float deltaTime)
{
	float speed = kirby.getSpeed();
	float horizontalVelocity = 0.f;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		horizontalVelocity -= speed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		horizontalVelocity += speed;
	}
	// The walking state now controls horizontal velocity.
	kirby.setVelocity({ horizontalVelocity, 0.f });
}