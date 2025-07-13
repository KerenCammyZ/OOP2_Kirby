#include "States/KirbyStates/KirbySparkAttackState.h"
#include "States/KirbyStates/KirbyStandingState.h"
#include "GameObj/MovingObj/Kirby.h"
#include "GameObj/MovingObj/Enemy.h"
#include <SFML/Window/Keyboard.hpp>
#include <cmath> // For std::sqrt and std::min

KirbySparkAttackState::KirbySparkAttackState(Kirby& kirby, std::vector<std::unique_ptr<Enemy>>& enemies)
	: m_enemies(enemies), m_isCharging(false), m_chargeTime(0.f)
{
	// Configure the visual for the spark aura
	m_sparkAura.setRadius(0.f);
	m_sparkAura.setFillColor(sf::Color(255, 255, 0, 100)); // Yellow, semi-transparent
	m_sparkAura.setOutlineColor(sf::Color::White);
	m_sparkAura.setOutlineThickness(2.f);
	m_sparkAura.setOrigin(0.f, 0.f); // We will update the origin as the radius changes
}

void KirbySparkAttackState::enter(Kirby& kirby)
{
	// When entering this state, immediately start charging the attack.
	m_isCharging = true;
	m_chargeTime = 0.f;
	kirby.setVelocity({ 0.f, 0.f }); // Stop Kirby from moving while charging

	kirby.setAnimation("spark_attack"); // Set the animation for the spark attack
}

std::unique_ptr<KirbyState> KirbySparkAttackState::handleInput(Kirby& kirby)
{
	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
	{
		// The attack is finished, transition back to the standing state.
		return std::make_unique<KirbyStandingState>();
	}

	// Stay in this state while the key is held down.
	return nullptr;
}

void KirbySparkAttackState::update(Kirby& kirby, float deltaTime)
{
	// While charging, Kirby can't move.
	kirby.setVelocity({ 0.f, 0.f });

	if (m_isCharging)
	{
		m_chargeTime += deltaTime;

		// The aura grows over time, up to a maximum size.
		float maxRadius = 120.f;
		float growthRate = 150.f;
		float newRadius = std::min(m_chargeTime * growthRate, maxRadius);

		m_sparkAura.setRadius(newRadius);
		m_sparkAura.setOrigin(newRadius, newRadius); // Keep the origin centered on the circle
		m_sparkAura.setPosition(kirby.getPosition());

		// Move the collision check from handleInput to update.
		// This loop now runs every frame, constantly checking the growing aura.
		for (auto& enemy : m_enemies)
		{
			// Skip enemies that are already defeated.
			if (!enemy || enemy->isSwallowed()) continue;

			sf::Vector2f vectorToEnemy = enemy->getPosition() - kirby.getPosition();
			float distance = std::sqrt(vectorToEnemy.x * vectorToEnemy.x + vectorToEnemy.y * vectorToEnemy.y);

			// If an enemy is inside the aura, defeat it instantly.
			if (distance < m_sparkAura.getRadius())
			{
				enemy->onSwallowed();
			}
		}
	}
}

void KirbySparkAttackState::draw(sf::RenderTarget& target)
{
	// Draw the visual aura if we are charging.
	if (m_isCharging)
	{
		target.draw(m_sparkAura);
	}
}