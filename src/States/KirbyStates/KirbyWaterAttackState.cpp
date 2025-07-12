#include "States/KirbyStates/KirbyWaterAttackState.h"
#include "States/KirbyStates/KirbySwimmingState.h"
#include "GameObj/MovingObj/Kirby.h"
#include "GameObj/MovingObj/Enemy.h"

KirbyWaterAttackState::KirbyWaterAttackState(Kirby& kirby, std::vector<std::unique_ptr<Enemy>>& enemies)
	: m_enemies(enemies), m_stateDuration(0.3f) // A quick, 0.3-second burst
{
	// Configure the visual and hitbox for the water stream.
	float streamLength = 70.f; // How far the water shoots
	float streamHeight = 12.f; // How thick the stream is
	m_waterStreamHitbox.setSize({ streamLength, streamHeight });
	m_waterStreamHitbox.setFillColor(sf::Color(120, 180, 255, 180)); // A nice water-blue
}

void KirbyWaterAttackState::enter(Kirby& kirby)
{
	// You could play a "spit" sound effect here.
	// We stop Kirby's movement to make the attack feel more deliberate.
	kirby.setVelocity({ 0.f, kirby.getVelocity().y });
}

std::unique_ptr<KirbyState> KirbyWaterAttackState::handleInput(Kirby& kirby)
{
	// This attack cannot be interrupted. It finishes when its duration is up.
	if (m_stateDuration <= 0.f)
	{
		return std::make_unique<KirbySwimmingState>();
	}
	return nullptr;
}

void KirbyWaterAttackState::update(Kirby& kirby, float deltaTime)
{
	// Countdown the attack's duration.
	m_stateDuration -= deltaTime;

	// Position the hitbox in front of Kirby each frame.
	sf::Vector2f position = kirby.getPosition();
	float offsetX = (kirby.getFacingDirection() == FacingDirection::Right) ?
		(kirby.getSize().x / 2.f) :
		(-m_waterStreamHitbox.getSize().x - (kirby.getSize().x / 2.f));

	m_waterStreamHitbox.setPosition(position.x + offsetX, position.y - (m_waterStreamHitbox.getSize().y / 2.f));

	// Check for collisions with enemies.
	for (auto& enemy : m_enemies)
	{
		// Skip enemies that are already defeated or being swallowed.
		if (enemy && !enemy->isSwallowed() && enemy->getState() != EnemyState::BEING_SWALLOWED)
		{
			if (m_waterStreamHitbox.getGlobalBounds().intersects(enemy->getBounds()))
			{
				// When the water stream hits an enemy, stun them.
				enemy->stun(2.0f);
			}
		}
	}
}

void KirbyWaterAttackState::draw(sf::RenderTarget& target)
{
	// Draw the visual for the water stream.
	target.draw(m_waterStreamHitbox);
}