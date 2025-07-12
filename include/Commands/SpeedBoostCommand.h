#pragma once
#include "Commands/PowerUpCommand.h"
#include "GameObj/MovingObj/Kirby.h" // Include Kirby to modify him

class SpeedBoostCommand : public PowerUpCommand
{
public:
	void execute(Kirby& kirby) override
	{
		// You may need to add get/setOriginalSpeed methods to Kirby
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
		return sf::seconds(60.f);
	}
};