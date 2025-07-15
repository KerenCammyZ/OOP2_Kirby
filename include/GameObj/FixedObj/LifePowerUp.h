#pragma once
#include "GameObj/FixedObj/PowerUp.h"
#include "GameObjectFactory.h"

class LifePowerUp : public PowerUp
{
public:
	void applyEffect(Kirby* kirby) override;
	ObjectType getType() const override { return ObjectType::POWERUP; }

private:
	// Static member for factory registration
	static bool m_registerLifePowerUp;
};