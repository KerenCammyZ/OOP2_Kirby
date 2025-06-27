#pragma once
#include "GameObj/FixedObj/FixedObject.h"

class Present : public FixedObject
{
public:
	Present() : m_isCollected(false)
	{
		// Make the debug shape visible and give it a unique color
		m_collisionShape.setFillColor(sf::Color(0, 255, 255, 100)); // Semi-transparent cyan
	}

	// This is the function each specific present will override
	virtual void applyEffect(Kirby* kirby) = 0;

	// When Kirby collides, apply the effect and mark for deletion
	void handleCollision(Kirby* kirby) override final;

	// Presents don't collide with doors
	void handleCollision(Door* door) override {};

	bool isCollected() const { return m_isCollected; }

	// --- NEW: Override the draw function for debugging ---
	void draw(sf::RenderTarget& target) const override
	{
		// First, call the base GameObject draw to try and draw the sprite
		GameObject::draw(target);
		// Then, explicitly draw the collision shape on top
		target.draw(m_collisionShape);
	}

private:
	bool m_isCollected;
};