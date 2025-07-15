#pragma once
#include "PowerUp.h" // The base class
#include "GameObjectFactory.h" // For factory registration

class SpeedPowerUp : public PowerUp
{
public:
	// The specific effect for this PowerUp
	void applyEffect(Kirby* kirby) override;
	ObjectType getType() const { return ObjectType::POWERUP; }
	//void draw();


private:
	static bool m_registerSpeedPowerUp;
};