//#include "States/KirbyStandingState.h"
//#include "States/KirbyWalkingState.h" // Use the new WalkingState
//#include "States/KirbyJumpingState.h" // Include the JumpingState header
#include "States/KirbyStandingState.h"
#include "States/KirbyWalkingState.h"
#include "States/KirbyJumpingState.h"
#include "GameObj/MovingObj/Kirby.h" // Kirby header is needed for update
#include <SFML/Window/Keyboard.hpp>

std::unique_ptr<KirbyState> KirbyStandingState::handleInput(Kirby& kirby)
{
	// Pressing UP transitions to Jumping state
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		return std::make_unique<KirbyJumpingState>();
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) ||
		sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		return std::make_unique<KirbyWalkingState>();
	}
	return nullptr;
}

void KirbyStandingState::update(Kirby& kirby, float deltaTime)
{
	// **THIS IS THE FIX**:
	// When standing, Kirby should have zero velocity. This prevents
	// him from sliding and stops gravity from accumulating.
	kirby.setVelocity({ 0.f, 0.f });
}