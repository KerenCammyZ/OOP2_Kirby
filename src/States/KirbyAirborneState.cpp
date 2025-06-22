#include "States/KirbyAirborneState.h"
#include "States/KirbyStandingState.h" // To transition to when we land
#include "GameObj/MovingObj/Kirby.h"
#include <SFML/Window/Keyboard.hpp>
#include "GlobalSizes.h";

std::unique_ptr<KirbyState> KirbyAirborneState::handleInput(Kirby& kirby)
{
	// If the collision check has set our state to grounded,
	// it's time to transition back to the standing state.
	if (kirby.isGrounded())
	{
		return std::make_unique<KirbyStandingState>();
	}
	return nullptr;
}

void KirbyAirborneState::update(Kirby& kirby, float deltaTime)
{
	// 1. Apply Gravity
	sf::Vector2f currentVelocity = kirby.getVelocity();
	currentVelocity.y += GRAVITY * deltaTime;
	kirby.setVelocity(currentVelocity);

	// 2. Handle Horizontal Air Control
	float speed = kirby.getSpeed() * 0.9f; // Slight speed reduction in air
	float horizontalVelocity = 0.f;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		horizontalVelocity -= speed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		horizontalVelocity += speed;
	}

	kirby.setVelocity({ horizontalVelocity, kirby.getVelocity().y });
}