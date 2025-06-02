#pragma once
#include "MovingObject.h"

class Kirby : public MovingObject
{
public:
	Kirby();
	void move(float deltaTime) override;
	void inhale();
	void swallow();
	void spit();
	void fly();

};
