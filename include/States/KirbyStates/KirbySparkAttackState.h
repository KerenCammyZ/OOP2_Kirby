#pragma once
#include "States/KirbyStates/KirbyState.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

class Enemy; // Forward-declaration

// This state manages the logic for Kirby's Spark charge attack.
class KirbySparkAttackState : public KirbyState
{
public:
	// The constructor takes a reference to the list of all enemies.
	KirbySparkAttackState(Kirby& kirby, std::vector<std::unique_ptr<Enemy>>& enemies);

	// Overridden functions from KirbyState
	void enter(Kirby& kirby) override;
	std::unique_ptr<KirbyState> handleInput(Kirby& kirby) override;
	void update(Kirby& kirby, float deltaTime) override;
	void draw(sf::RenderTarget& target) override;

private:
	// Member variables to manage the attack
	std::vector<std::unique_ptr<Enemy>>& m_enemies; // Reference to the game's enemy list
	sf::CircleShape m_sparkAura; // The visual for the AoE attack
	bool m_isCharging;
	float m_chargeTime;
};