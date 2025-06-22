#pragma once
#include "States/KirbyAirborneState.h"

class KirbyJumpingState : public KirbyAirborneState
{
public:
	// The only unique behavior is what happens on entry: the jump itself.
	void enter(Kirby& kirby) override;
};