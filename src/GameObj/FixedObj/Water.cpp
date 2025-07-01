#include "GameObj/FixedObj/Water.h"
#include "GameObj/MovingObj/Kirby.h"
#include "GameObjectFactory.h"
#include <SFML/Graphics.hpp>
#include <iostream>

const sf::Color waterColor(0, 0, 200);

// Define and initialize the static registration member.
// This will call the factory's registerType function once when the program starts.
bool Water::m_register = GameObjectFactory::registerType(
	waterColor,
	[](sf::Vector2f position, const Kirby* kirby) -> std::unique_ptr<GameObject>
	{
		return std::make_unique<Water>();
	}
);

void Water::handleCollision(Kirby* kirby)
{
	// This is the core logic. When Kirby is inside a Water object,
	// we set his internal "in water" flag to true.
	kirby->setInWater(true);
	
}