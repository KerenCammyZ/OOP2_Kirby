#pragma once
#include <vector>
#include <memory>
#include <SFML/System/Clock.hpp>
#include "Commands/PowerUpCommand.h" 

class Kirby;

class PowerUpManager
{
public:
	void add(std::unique_ptr<PowerUpCommand> command, Kirby& kirby);
	void update(float deltaTime, Kirby& kirby);
	PowerUpCommand getLastPowerUpEffect();

private:
	struct ActivePowerUpEffect
	{
		std::unique_ptr<PowerUpCommand> command;
		sf::Clock timer;
	};

	std::vector<ActivePowerUpEffect> m_activeEffects;
};