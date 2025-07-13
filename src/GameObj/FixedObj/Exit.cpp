#include "GameObj/FixedObj/Exit.h"
#include "GameObjectFactory.h"
#include "GlobalSizes.h"

const sf::Color exitColor = sf::Color(0, 255, 255);

static bool isExitRegistered = GameObjectFactory::registerType(
	exitColor,
	[](sf::Vector2f position, Kirby* kirby) -> std::unique_ptr<GameObject>
	{
		return std::make_unique<Exit>();
	}
);