// Floor.h
#pragma once
#include "GameObj/FixedObj/FixedObject.h"

class Door;

class Floor : public FixedObject
{
public:
	Floor() = default;

	void handleCollision(Kirby* kirby) override;
	void handleCollision(Enemy* enemy) override;

private:
	static bool m_registeritem;
};