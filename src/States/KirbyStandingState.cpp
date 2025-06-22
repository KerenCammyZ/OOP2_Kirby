#include "States/KirbyStandingState.h"
#include "States/KirbyMovingState.h" // To transition to the moving state
#include <SFML/Window/Keyboard.hpp>

std::unique_ptr<KirbyState> KirbyStandingState::handleInput()
{
	// If any movement key is pressed, transition to the Moving state.
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) ||
		sf::Keyboard::isKeyPressed(sf::Keyboard::Right) ||
		sf::Keyboard::isKeyPressed(sf::Keyboard::Up) ||
		sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		return std::make_unique<KirbyMovingState>();
	}

	// Otherwise, remain in the Standing state.
	return nullptr;
}

void KirbyStandingState::update(Kirby& kirby, float deltaTime)
{
	// Kirby doesn't move when standing, so this is empty.
	// You could add idle animations here later.
}