#pragma once
#include "Commands/PresentCommand.h"
#include "GameObj/MovingObj/Kirby.h" // Include Kirby to modify him

class SpeedBoostCommand : public PresentCommand
{
public:
	void execute(Kirby& kirby) override
	{
		// You may need to add get/setOriginalSpeed methods to Kirby
		kirby.setSpeed(kirby.getOriginalSpeed() * 1.5f);
	}

	void undo(Kirby& kirby) override
	{
		kirby.setSpeed(kirby.getOriginalSpeed());
	}

	sf::Time getDuration() const override
	{
		return sf::seconds(60.f);
	}
};