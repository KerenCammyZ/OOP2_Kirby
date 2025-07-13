#pragma once
#include "GameObj/FixedObj/FixedObject.h"
#include "ResourceManager.h"

class PowerUp : public FixedObject
{
public:
	PowerUp() : m_isCollected(false)
	{
		// Make the debug shape visible and give it a unique color
		m_collisionShape.setFillColor(sf::Color(0, 255, 255, 100)); // Semi-transparent cyan
	}

	// This is the function each specific PowerUp will override
	virtual void applyEffect(Kirby* kirby) = 0;

	// When Kirby collides, apply the effect and mark for deletion
	void handleCollision(Kirby* kirby) override final;

	// PowerUps don't collide with doors
	void handleCollision(Door* door) override {};

	void handleCollision(Spike* spike) override {};

	bool isCollected() const { return m_isCollected; }


private:
	bool m_isCollected;
};