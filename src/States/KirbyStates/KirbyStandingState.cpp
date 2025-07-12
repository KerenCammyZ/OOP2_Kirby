#include "States/KirbyStates/KirbyStandingState.h"
#include "States/KirbyStates/KirbyWalkingState.h"
#include "States/KirbyStates/KirbyJumpingState.h"
#include "GameObj/MovingObj/Kirby.h"
#include <SFML/Window/Keyboard.hpp>

std::unique_ptr<KirbyState> KirbyStandingState::handleInput(Kirby& kirby)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		return std::make_unique<KirbyJumpingState>();
		kirby.setFacingDirection(FacingDirection::Left);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) ||
		sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		return std::make_unique<KirbyWalkingState>();
		kirby.setFacingDirection(FacingDirection::Right);
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