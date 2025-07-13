#include "GameObj/FixedObj/Spike.h"
#include "GameObj/MovingObj/Kirby.h"
#include "GameObj/MovingObj/Enemy.h"
#include "GameObjectFactory.h"

// Choose a unique color for spikes in your collision map.
// This example uses pure black.
const sf::Color spikeColor(0, 0, 0);

// Register the Spike object with the factory.
bool Spike::m_register = GameObjectFactory::registerType(
	spikeColor,
	[](sf::Vector2f position, Kirby* kirby) -> std::unique_ptr<GameObject>
	{
		return std::make_unique<Spike>();
	}
);

void Spike::handleCollision(Kirby* kirby)
{
	kirby->handleCollision(this);
}

void Spike::handleCollision(Enemy* enemy)
{
	;
}