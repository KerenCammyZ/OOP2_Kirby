#include "GameObj/FixedObj/SpeedPresent.h"
#include "Commands/SpeedBoostCommand.h" // Include the command it will issue
#include <iostream>

// Static lambda to register this present with the factory
static bool isSpeedPresentRegistered = GameObjectFactory::instance().registerType(
	SPEED_PRESENT_COLOR,
	[](sf::Vector2f position) -> std::unique_ptr<GameObject>
	{
		auto present = std::make_unique<SpeedPresent>();

		// --- NEW: Load and set the texture for the present ---
		auto presentTexture = std::make_shared<sf::Texture>();
		if (presentTexture->loadFromFile("SpeedPresent.png"))
		{
			present->setTexture(presentTexture);
			// --- THIS IS THE FIX ---
			// After setting the texture, we must also set the size.
			// This forces the sprite's scale to be calculated correctly.
			present->setSize({ ENTITY_SIZE, ENTITY_SIZE });
		}
		else
		{
			// Optional: Log an error if the texture can't be found
			 std::cerr << "Failed to load SpeedPresent.png\n";
		}

		return present;
	}
);
	
void SpeedPresent::applyEffect(Kirby* kirby)
{
	// Create a command and give it to Kirby's manager
	kirby->addPresentEffect(std::make_unique<SpeedBoostCommand>());
}
