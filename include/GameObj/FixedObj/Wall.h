// Wall.cpp
#pragma once
#include "GameObj/FixedObj/StaticObject.h"

class Kirby;

class Wall : public StaticObject
{
public:
	Wall() = default;

	void handleCollision(Kirby* kirby) override;
private:
};