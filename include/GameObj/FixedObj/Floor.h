// Floor.h
#pragma once
#include "GameObj/FixedObj/StaticObject.h"

class Door;

class Floor : public StaticObject
{
public:
	Floor() = default;

	void handleCollision(Kirby* kirby) override;
	void handleCollision(Door* door) override {};
private:

};