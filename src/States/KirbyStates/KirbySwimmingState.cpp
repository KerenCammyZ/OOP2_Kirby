#include "States/KirbyStates/KirbySwimmingState.h"
#include "States/KirbyStates/KirbyFallingState.h" // To transition to when leaving water
#include "GameObj/MovingObj/Kirby.h"
#include <SFML/Window/Keyboard.hpp>
#include "GlobalSizes.h"
#include <iostream> // for debugging

void KirbySwimmingState::enter(Kirby& kirby)
{
	std::cout << "entered swimming state\n";
	// When entering water, kill any existing vertical velocity from a jump/fall.
	kirby.setVelocity({ kirby.getVelocity().x, 0.f });
}

std::unique_ptr<KirbyState> KirbySwimmingState::handleInput(Kirby& kirby)
{
	// If the collision checks this frame determine we are no longer in water...
	if (!kirby.isInWater())
	{
		// ...transition to the falling state to fall back to the ground.
		return std::make_unique<KirbyFallingState>();
	}
	return nullptr;
}

void KirbySwimmingState::update(Kirby& kirby, float deltaTime)
{
	//std::cout << "Kirby is updating in swimming state\n";
	// 1. Apply Water Physics
	sf::Vector2f velocity = kirby.getVelocity();
	// Gravity still applies, but buoyancy pushes upward.
	velocity.y += (GRAVITY + BUOYANCY) * deltaTime;
	// Water drag slows down movement.
	velocity *= (1.0f - WATER_DRAG * deltaTime);

	// 2. Handle Swimming Controls
	float swimSpeed = kirby.getSpeed() * 0.75f; // Swimming is a bit slower

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		velocity.x = -swimSpeed - BUOYANCY;
		kirby.setFacingDirection(FacingDirection::Left);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		velocity.x = swimSpeed + BUOYANCY;;
		kirby.setFacingDirection(FacingDirection::Right);
	}

	// Pressing UP makes Kirby swim upwards
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		velocity.y = -swimSpeed - BUOYANCY;
	}

	//Pressing DOWN makes Kirby swim downwards
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		velocity.y = swimSpeed + BUOYANCY;
	}

	kirby.setVelocity(velocity);
}
