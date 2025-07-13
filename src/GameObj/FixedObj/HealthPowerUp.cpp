#include "GameObj/FixedObj/HealthPowerUp.h"
#include "Commands/HealthCommand.h" // Include the new command
#include "GameObj/MovingObj/Kirby.h"
#include <iostream>

// Choose a new, unique color for the health power-up in your collision map.
// This example uses Hot Pink.
const sf::Color healthPowerUpColor(255, 105, 180);

// Register this new object with the GameObjectFactory.
bool HealthPowerUp::m_registerHealthPowerUp = GameObjectFactory::registerType(
	healthPowerUpColor,
	[](sf::Vector2f position, Kirby* kirby) -> std::unique_ptr<GameObject>
	{
		auto powerUp = std::make_unique<HealthPowerUp>();

		// You will need to create a "HealthPowerUp.png" sprite for this.
		auto powerUpTexture = std::make_shared<sf::Texture>();
		if (powerUpTexture->loadFromFile("HealthPowerUp.png"))
		{
			powerUp->setTexture(powerUpTexture);
			powerUp->setSize({ ENTITY_SIZE, ENTITY_SIZE });
		}
		else
		{
			std::cerr << "Failed to load HealthPowerUp.png\n";
		}

		return powerUp;
	}
);

void HealthPowerUp::applyEffect(Kirby* kirby)
{
	// When Kirby collides, create the instant health command
	// and give it to his manager.
	kirby->addPowerUpEffect(std::make_unique<HealthCommand>());
}