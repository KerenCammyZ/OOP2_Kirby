#include "GameObj/FixedObj/SpeedPowerUp.h"
#include "Commands/SpeedBoostCommand.h" // Include the command it will issue
#include <iostream>

const sf::Color speedPowerUpColor(255, 216, 0);

// Static lambda to register this PowerUp with the factory
bool SpeedPowerUp::m_registerSpeedPowerUp = GameObjectFactory::registerType(
	speedPowerUpColor,
	[](sf::Vector2f position, Kirby* kirby) -> std::unique_ptr<GameObject>
	{
		auto PowerUp = std::make_unique<SpeedPowerUp>();

		// --- NEW: Load and set the texture for the PowerUp ---
		auto PowerUpTexture = std::make_shared<sf::Texture>();


		if (!PowerUpTexture->loadFromFile("SpeedPowerUp.png")) {
			std::cerr << "Failed to load SpeedPowerUp texture" << std::endl;
			throw std::runtime_error("Failed to load SpeedPowerUp texture");
		}
		PowerUp->setTexture(PowerUpTexture);
		// --- THIS IS THE FIX ---
		// After setting the texture, we must also set the size.
		// This forces the sprite's scale to be calculated correctly.
		PowerUp->setSize({ ENTITY_SIZE, ENTITY_SIZE });

		return PowerUp;
	}
);

void SpeedPowerUp::applyEffect(Kirby* kirby)
{
	// Create a command and give it to Kirby's manager
	kirby->addPowerUpEffect(std::make_unique<SpeedBoostCommand>());
}