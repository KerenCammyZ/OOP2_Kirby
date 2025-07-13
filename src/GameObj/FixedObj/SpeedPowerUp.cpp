#include "GameObj/FixedObj/SpeedPowerUp.h"
#include "Commands/SpeedBoostCommand.h" // Include the command it will issue
#include <iostream>

const sf::Color speedPowerUpColor(255, 216, 0);

// Static lambda to register this PowerUp with the factory
bool SpeedPowerUp::m_registerSpeedPowerUp = GameObjectFactory::registerType(
	speedPowerUpColor,
	[](sf::Vector2f position,  Kirby* kirby) -> std::unique_ptr<GameObject>
	{
		auto PowerUp = std::make_unique<SpeedPowerUp>();

		// --- NEW: Load and set the texture for the PowerUp ---
		auto PowerUpTexture = std::make_shared<sf::Texture>();

		// Use ResourceManager to get the texture
		try {
			auto PowerUpTexture = ResourceManager::getInstance().getTexture("SpeedPowerUp.png");
			PowerUp->setTexture(PowerUpTexture);
			// --- THIS IS THE FIX ---
			// After setting the texture, we must also set the size.
			// This forces the sprite's scale to be calculated correctly.
			PowerUp->setSize({ ENTITY_SIZE, ENTITY_SIZE });
		}
		catch (const std::exception& e) {
			std::cerr << "Failed to load SpeedPowerUp.png: " << e.what() << std::endl;
		}

		return PowerUp;
	}
);

void SpeedPowerUp::applyEffect(Kirby* kirby)
{
	// Create a command and give it to Kirby's manager
	kirby->addPowerUpEffect(std::make_unique<SpeedBoostCommand>());
}