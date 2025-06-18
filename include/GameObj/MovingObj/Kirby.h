// Kirby.h
#pragma once
#include "GameObj/MovingObj/MovingObject.h"

class Kirby : public MovingObject
{
public:
	Kirby(std::shared_ptr<sf::Texture>& kirbyTexture);
	void move(float deltaTime) override;

	// Collision Handlers
	void handleCollision(GameObject* other) override;
	void handleCollision(Kirby* kirby) override {};
	void handleCollision(Door* door) override;
};
