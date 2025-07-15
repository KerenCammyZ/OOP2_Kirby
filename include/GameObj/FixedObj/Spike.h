#pragma once
#include "GameObj/FixedObj/FixedObject.h"

class Spike : public FixedObject
{
public:
	Spike() = default;

	// Collision Handlers
	void handleCollision(Kirby* kirby) override;
	void handleCollision(Enemy* enemy) override;


	// Return the unique type for this object.
	ObjectType getType() const override { return ObjectType::SPIKE; }

	// Spikes are invisible collision objects, so draw does nothing.
	void draw(sf::RenderTarget& target) const override {};

private:
	// Static member for factory registration.
	static bool m_register;
};