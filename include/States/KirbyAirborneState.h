#pragma once
#include "States/KirbyState.h"

class KirbyAirborneState : public KirbyState
{
public:
	// Handle player's mid-air left/right control
	virtual void update(Kirby& kirby, float deltaTime) override;

	// Check if we have landed on the ground
	virtual std::unique_ptr<KirbyState> handleInput(Kirby& kirby) override;
};