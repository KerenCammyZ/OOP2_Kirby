// Wall.cpp
#pragma once
#include "GameObj/FixedObj/FixedObject.h"

class Kirby;

class Wall : public FixedObject
{
public:
	Wall() = default;

	void handleCollision(Kirby* kirby) override;
private:
};