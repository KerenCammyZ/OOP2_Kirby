// Door.h
#pragma once
#include "GameObj/FixedObj/FixedObject.h"

class Door : public FixedObject
{
public:
	// The constructor now takes the destination coordinates
	Door(sf::Vector2f destination);

	void handleCollision(Kirby* kirby) override;
	void handleCollision(Door* door) override {};

private:
	sf::Vector2f m_destination; // Where this door leads
};