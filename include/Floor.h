// StaticObject.h
#pragma once
#include "StaticObject.h"

class Kirby;
class Floor : public StaticObject
{
public:
	Floor() = default;

	void handleCollision(Kirby* kirby) override;
private:

};

