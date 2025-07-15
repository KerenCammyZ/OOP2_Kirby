// Door.h
#pragma once
#include "GameObj/FixedObj/FixedObject.h"

class Door : public FixedObject
{
public:
	Door(sf::Vector2f destination);

	void handleCollision(Kirby* kirby) override;

private:
	sf::Vector2f m_destination; // Where this door leads
	static bool m_registeritem; // Static member to register this type with the factory
};