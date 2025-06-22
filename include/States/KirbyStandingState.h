#pragma once
#include "States/KirbyState.h"

class KirbyStandingState : public KirbyState
{
public:
	std::unique_ptr<KirbyState> handleInput() override;
	void update(Kirby& kirby, float deltaTime) override;
};