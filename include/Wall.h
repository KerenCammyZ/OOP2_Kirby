// Wall.h
#pragma once
#include "StaticObject.h"

class Wall : public StaticObject
{
public:
	Wall() = default;

	void handleCollision(Kirby* kirby) override;
private:
};