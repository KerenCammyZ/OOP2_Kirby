#pragma once
#include "States/KirbyStates/KirbyState.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

class Enemy;

// This state manages the logic for Kirby's Spark charge attack.
class KirbySparkAttackState : public KirbyState
{
public:
	KirbySparkAttackState(Kirby& kirby, std::vector<std::unique_ptr<Enemy>>& enemies);

	// Overridden functions from KirbyState
	void enter(Kirby& kirby) override;
	std::unique_ptr<KirbyState> handleInput(Kirby& kirby) override;
	void update(Kirby& kirby, float deltaTime) override;
	void draw(sf::RenderTarget& target) override;

private:
	bool m_isCharging;
	float m_chargeTime;
	sf::CircleShape m_sparkAura; // The visual for the AoE attack
	std::vector<std::unique_ptr<Enemy>>& m_enemies;
};