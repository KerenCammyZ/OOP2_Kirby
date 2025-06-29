//#pragma once
//
//#include "GameObj/FixedObj/FixedObject.h"
//
//class Water : public FixedObject
//{
//public:
//	Water() = default;
//
//	// When Kirby collides with this object, we'll tell him he's in water.
//	void handleCollision(Kirby* kirby) override;
//
//	void handleCollision(Door* door) override {};
//	ObjectType getType() const override { return ObjectType::WATER; }
//
//	// Override draw to do nothing, making the water zone invisible.
//	void draw(sf::RenderTarget& target) const override {};
//
//private:
//};

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