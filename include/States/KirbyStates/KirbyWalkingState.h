#pragma once
#include "States/KirbyStates/KirbyState.h"

class KirbyWalkingState : public KirbyState
{
public:
	std::unique_ptr<KirbyState> handleInput(Kirby& kirby) override;
	void update(Kirby& kirby, float deltaTime) override;
};