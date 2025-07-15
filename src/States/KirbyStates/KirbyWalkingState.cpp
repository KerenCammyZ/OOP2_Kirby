#include "States/KirbyStates/KirbyWalkingState.h"
#include "States/KirbyStates/KirbyStandingState.h"
#include "States/KirbyStates/KirbyJumpingState.h"
#include "States/KirbyStates/KirbyFallingState.h"
#include "GameObj/MovingObj/Kirby.h"
#include <SFML/Window/Keyboard.hpp>

#include <iostream>

std::unique_ptr<KirbyState> KirbyWalkingState::handleInput(Kirby& kirby)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		return std::make_unique<KirbyJumpingState>();
	}

	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Left) &&
		!sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		return std::make_unique<KirbyStandingState>();
	}

	if(!kirby.isGrounded())
	{
		return std::make_unique<KirbyFallingState>();
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
		kirby.setFacingDirection(FacingDirection::Left);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		horizontalVelocity += speed;
		kirby.setFacingDirection(FacingDirection::Right);
	}

	// The walking state now controls horizontal velocity.
	kirby.setVelocity({ horizontalVelocity, 0.f });
}

void KirbyWalkingState::enter(Kirby& kirby)
{
	kirby.setAnimation("walking");
}

