#include "GameObj/FixedObj/LifePowerUp.h"
#include "Commands/LifeCommand.h" // Include the new command
#include "GameObj/MovingObj/Kirby.h"
#include <iostream>


const sf::Color lifePowerUpColor(0, 122, 0); // bright, pure green.

// Register this new object with the GameObjectFactory.
bool LifePowerUp::m_registerLifePowerUp = GameObjectFactory::registerType(
	lifePowerUpColor,
	[](sf::Vector2f position, Kirby* kirby) -> std::unique_ptr<GameObject>
	{
		auto powerUp = std::make_unique<LifePowerUp>();

		auto powerUpTexture = ResourceManager::getInstance().getTexture("LifePowerUp.png");
		if (powerUpTexture)
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
	// When Kirby collides with this power-up, apply the life effect
	kirby->addPowerUpEffect(std::make_unique<LifeCommand>());
}