#pragma once
#include <vector>
#include <memory>
#include <SFML/System/Clock.hpp>
#include "Commands/PresentCommand.h" 

class Kirby;

class PresentManager
{
public:
	void add(std::unique_ptr<PresentCommand> command, Kirby& kirby);
	void update(float deltaTime, Kirby& kirby);

private:
	struct ActivePresentEffect
	{
		std::unique_ptr<PresentCommand> command;
		sf::Clock timer;
	};

	std::vector<ActivePresentEffect> m_activeEffects;
};