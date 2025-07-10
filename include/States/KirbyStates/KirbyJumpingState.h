#pragma once
#include "States/KirbyStates/KirbyAirborneState.h"

class KirbyJumpingState : public KirbyAirborneState
{
public:
	// The only unique behavior is what happens on entry: the jump itself.
	void enter(Kirby& kirby) override;
};