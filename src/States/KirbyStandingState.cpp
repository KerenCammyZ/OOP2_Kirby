#include "States/KirbyStandingState.h"
#include "States/KirbyWalkingState.h" // Use the new WalkingState
#include <SFML/Window/Keyboard.hpp>

std::unique_ptr<KirbyState> KirbyStandingState::handleInput()
{
	// If a HORIZONTAL movement key is pressed, transition to the Walking state.
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) ||
		sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		return std::make_unique<KirbyWalkingState>();
	}

	// We will add the check for the UP key here later to trigger a JUMP state.

	return nullptr;
}

void KirbyStandingState::update(Kirby& kirby, float deltaTime)
{
	// Kirby doesn't move when standing, so this is empty.
}