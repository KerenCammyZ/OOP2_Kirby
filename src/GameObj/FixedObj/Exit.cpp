#include "GameObj/FixedObj/Exit.h"
#include "GameObjectFactory.h"
#include "GlobalSizes.h"

// The static registration block for the new Exit object.
// The factory will now know how to build an Exit when it sees
// the cyan EXIT_COLOR in the collision map.

const sf::Color exitColor = sf::Color(0, 255, 255);

static bool isExitRegistered = GameObjectFactory::registerType(
	exitColor,
	[](sf::Vector2f position, Kirby* kirby) -> std::unique_ptr<GameObject>
	{
		return std::make_unique<Exit>();
	}
);