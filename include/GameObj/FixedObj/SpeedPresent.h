#pragma once
#include "Present.h" // The base class
#include "GameObjectFactory.h" // For factory registration
#include "GlobalSizes.h" // For colors

class SpeedPresent : public Present
{
public:
	// The specific effect for this present
	void applyEffect(Kirby* kirby) override;
	void draw();
};