#include "GameObj/FixedObj/LifePowerUp.h"
#include "Commands/LifeCommand.h" // Include the new command
#include "GameObj/MovingObj/Kirby.h"
#include <iostream>

// Choose a new, unique color for the life power-up in your collision map.
// This example uses a bright, pure green.
const sf::Color lifePowerUpColor(0, 122, 0);

// Register this new object with the GameObjectFactory.
bool LifePowerUp::m_registerLifePowerUp = GameObjectFactory::registerType(
	lifePowerUpColor,
	[](sf::Vector2f position, Kirby* kirby) -> std::unique_ptr<GameObject>
	{
		auto powerUp = std::make_unique<LifePowerUp>();

		// You will need to create a "LifePowerUp.png" sprite for this.
		auto powerUpTexture = std::make_shared<sf::Texture>();
		if (powerUpTexture->loadFromFile("LifePowerUp.png"))
		{
			powerUp->setTexture(powerUpTexture);
			powerUp->setSize({ ENTITY_SIZE, ENTITY_SIZE });
		}
		else
		{
			std::cerr << "Failed to load LifePowerUp.png\n";
		}

		return powerUp;
	}
);

void LifePowerUp::applyEffect(Kirby* kirby)
{
	// When Kirby collides, create the instant life command
	// and give it to his manager.
	kirby->addPowerUpEffect(std::make_unique<LifeCommand>());
}
