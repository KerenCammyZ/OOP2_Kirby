#pragma once
#include "Commands/PowerUpCommand.h"
#include "GameObj/MovingObj/Kirby.h"

class LifeCommand : public PowerUpCommand
{
public:
	void execute(Kirby& kirby) override
	{
		// Tell Kirby to add one life
		kirby.addLife(1);
	}

	// Since this is an instant effect, undo does nothing.
	void undo(Kirby& kirby) override {}

	// A zero duration makes this an instant-use item.
	sf::Time getDuration() const override
	{
		return sf::Time::Zero;
	}
};