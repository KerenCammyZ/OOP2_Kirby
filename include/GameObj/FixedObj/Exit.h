#pragma once
#include "GameObj/FixedObj/FixedObject.h"

// The Exit object is a simple "trigger". It doesn't need any special logic.
// The GameController will see that Kirby has hit an object of type EXIT
// and will handle advancing the level.
class Exit : public FixedObject
{
public:
	Exit() = default;

	// This can be empty because the controller handles the logic.
	void handleCollision(Kirby* kirby) override {};

	// Fulfills the abstract base class requirement.
	//void handleCollision(Door* door) override {};

	// Returns its unique type.
	ObjectType getType() const override { return ObjectType::EXIT; }
};