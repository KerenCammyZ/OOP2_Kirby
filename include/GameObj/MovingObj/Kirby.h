// Kirby.h
#pragma once
#include "GameObj/MovingObj/MovingObject.h"
#include <memory> // Add memory for std::unique_ptr

class KirbyState; // Forward-declare the base state class

class Kirby : public MovingObject
{
public:
	Kirby(std::shared_ptr<sf::Texture>& kirbyTexture);
	void move(float deltaTime) override;

	// Add a public getter for speed so states can access it
	float getSpeed() const;

	// Collision Handlers
	void handleCollision(GameObject* other) override;
	void handleCollision(Kirby* kirby) override {};
	void handleCollision(Door* door) override;

private:
	// Kirby now owns its current state
	std::unique_ptr<KirbyState> m_state;
};
