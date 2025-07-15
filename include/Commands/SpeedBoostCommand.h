#pragma once
#include "Commands/PowerUpCommand.h"
#include "GameObj/MovingObj/Kirby.h"

class SpeedBoostCommand : public PowerUpCommand
{
public:
	void execute(Kirby& kirby) override
	{
		kirby.setSpeed(kirby.getOriginalSpeed() * 1.5f);
		kirby.setHyper(true);
	}

	void undo(Kirby& kirby) override
	{
		kirby.setSpeed(kirby.getOriginalSpeed());
		kirby.setHyper(false);
	}

	sf::Time getDuration() const override
	{
		return sf::seconds(10.f);
	}
};