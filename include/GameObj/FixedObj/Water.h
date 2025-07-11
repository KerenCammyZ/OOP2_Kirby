#pragma once

#include "GameObj/FixedObj/FixedObject.h"

class Water : public FixedObject
{
public:
	Water() = default;

	void handleCollision(Kirby* kirby) override;
	void handleCollision(Door* door) override {};
	ObjectType getType() const override { return ObjectType::WATER; }

	// Override draw to do nothing, making the water zone invisible.
	void draw(sf::RenderTarget& target) const override {};

private:
	// The static member variable used to register this class with the factory.
	static bool m_register;
};

// IDEA FROM 12:30AM
/*
	create water bounding box using two water objects marked on collision map
	bada bing bada boom we got one object for lot's o water
*/