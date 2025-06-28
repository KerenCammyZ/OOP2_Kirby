#include "PresentManager.h"
#include "GameObj/MovingObj/Kirby.h"
#include <algorithm>

void PresentManager::add(std::unique_ptr<PresentCommand> command, Kirby& kirby)
{
	command->execute(kirby);
	m_activeEffects.push_back({ std::move(command), sf::Clock() });
}

void PresentManager::update(float deltaTime, Kirby& kirby)
{
	auto it = std::remove_if(m_activeEffects.begin(), m_activeEffects.end(),
		[&kirby](const ActivePresentEffect& effect)
		{
			if (effect.timer.getElapsedTime() >= effect.command->getDuration())
			{
				effect.command->undo(kirby);
				return true;
			}
			return false;
		});

	m_activeEffects.erase(it, m_activeEffects.end());
}