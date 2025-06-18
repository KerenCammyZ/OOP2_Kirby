//Floor.h
#pragma once
#include "GameObj/FixedObj/FixedObject.h"

class Kirby;
class Door;

class Floor : public FixedObject
{
public:
	Floor() = default;

	void handleCollision(Kirby* kirby) override;
	void handleCollision(Door* door) override {};
private:

};