#pragma once
#include "States/KirbyStates/KirbyState.h"

class KirbySwimmingState : public KirbyState
{
public:
	void enter(Kirby& kirby) override;
	std::unique_ptr<KirbyState> handleInput(Kirby& kirby) override;
	void update(Kirby& kirby, float deltaTime) override;
};