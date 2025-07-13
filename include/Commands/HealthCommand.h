#pragma once
#include "Commands/PowerUpCommand.h"
#include "GameObj/MovingObj/Kirby.h"

class HealthCommand : public PowerUpCommand
{
public:
	void execute(Kirby& kirby) override
	{
		// Heal Kirby by 2 health points. You can change this value.
		kirby.heal(2);
	}

	// Since this is an instant effect, undo does nothing.
	void undo(Kirby& kirby) override {}

	// A zero duration tells the PowerUpManager to execute this command
	// and then immediately discard it, which is perfect for an instant effect.
	sf::Time getDuration() const override
	{
		return sf::Time::Zero;
	}
};