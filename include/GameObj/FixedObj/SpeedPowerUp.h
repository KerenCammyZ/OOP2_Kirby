#pragma once
#include "PowerUp.h" // The base class
#include "GameObjectFactory.h" // For factory registration
#include "GlobalSizes.h" // For colors

class SpeedPowerUp : public PowerUp
{
public:
	// The specific effect for this PowerUp
	void applyEffect(Kirby* kirby) override;
	void draw();

	ObjectType getType() const { return ObjectType::SPEED_PowerUp; }

private:
	static bool m_registerSpeedPowerUp;
};