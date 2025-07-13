#pragma once
#include "GameObj/FixedObj/PowerUp.h"
#include "GameObjectFactory.h"

class HealthPowerUp : public PowerUp
{
public:
	void applyEffect(Kirby* kirby) override;

	// Add a new type for this object
	ObjectType getType() const override { return ObjectType::POWERUP; }

private:
	// Static member for factory registration
	static bool m_registerHealthPowerUp;
};