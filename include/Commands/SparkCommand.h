#pragma once
#include "Commands/PresentCommand.h"
#include "GameObj/MovingObj/Kirby.h"

// This command grants Kirby the Spark power-up for a limited time.
class SparkCommand : public PresentCommand
{
public:
	// When executed, give Kirby the Spark power.
	void execute(Kirby& kirby) override
	{
		kirby.setPower(PowerUpType::Spark);
	}

	// When the timer runs out, remove the power.
	void undo(Kirby& kirby) override
	{
		kirby.setPower(PowerUpType::None);
	}

	// The power-up will last for 60 seconds.
	sf::Time getDuration() const override
	{
		return sf::seconds(60.f);
	}
};