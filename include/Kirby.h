// Kirby.h
#pragma once
#include "MovingObject.h"

class Kirby : public MovingObject
{
public:
	Kirby();
	void move(float deltaTime) override;

	// Collision Handlers
	void handleCollision(GameObject* other) override;
	void handleCollision(Kirby* kirby) override {}; // Kirby hitting himself

};
