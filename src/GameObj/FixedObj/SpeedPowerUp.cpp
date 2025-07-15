// SpeedPowerUp.cpp
#include "GameObj/FixedObj/SpeedPowerUp.h"
#include "Commands/SpeedBoostCommand.h"
#include <iostream>


const sf::Color speedPowerUpColor(255, 216, 0);

bool SpeedPowerUp::m_registerSpeedPowerUp = GameObjectFactory::registerType(
	speedPowerUpColor,
	[](sf::Vector2f position, Kirby* kirby) -> std::unique_ptr<GameObject>
	{
		auto PowerUp = std::make_unique<SpeedPowerUp>();

		auto PowerUpTexture = ResourceManager::getInstance().getTexture("SpeedPowerUp.png");
		if (!PowerUpTexture) {
			std::cerr << "Failed to load SpeedPowerUp texture" << std::endl;
			throw std::runtime_error("Failed to load SpeedPowerUp texture");
		}
		PowerUp->setTexture(PowerUpTexture);
		PowerUp->setSize({ ENTITY_SIZE, ENTITY_SIZE });

		return PowerUp;
	}
);

// When Kirby collides with this power-up, apply the speed boost effect.
void SpeedPowerUp::applyEffect(Kirby* kirby)
{
	kirby->addPowerUpEffect(std::make_unique<SpeedBoostCommand>());
}