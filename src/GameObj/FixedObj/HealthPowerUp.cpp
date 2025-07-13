#include "GameObj/FixedObj/HealthPowerUp.h"
#include "Commands/HealthCommand.h"
#include "GameObj/MovingObj/Kirby.h"
#include <iostream>

const sf::Color healthPowerUpColor(255, 105, 180); // Hot Pink

bool HealthPowerUp::m_registerHealthPowerUp = GameObjectFactory::registerType(
	healthPowerUpColor,
	[](sf::Vector2f position, Kirby* kirby) -> std::unique_ptr<GameObject>
	{
		auto powerUp = std::make_unique<HealthPowerUp>();

		auto powerUpTexture = ResourceManager::getInstance().getTexture("HealthPowerUp.png");
		if (powerUpTexture)
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
	// When Kirby collides with this power-up, apply the health effect
	kirby->addPowerUpEffect(std::make_unique<HealthCommand>());
}