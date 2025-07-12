#pragma once
#include "States/KirbyStates/KirbyState.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

class Enemy; // Forward-declaration

// This state handles the logic for the short-range water fountain attack.
class KirbyWaterAttackState : public KirbyState
{
public:
	// The constructor needs access to the list of enemies to check for collisions.
	KirbyWaterAttackState(Kirby& kirby, std::vector<std::unique_ptr<Enemy>>& enemies);

	void enter(Kirby& kirby) override;
	std::unique_ptr<KirbyState> handleInput(Kirby& kirby) override;
	void update(Kirby& kirby, float deltaTime) override;

	// This state will draw the water stream itself.
	void draw(sf::RenderTarget& target) override;

private:
	std::vector<std::unique_ptr<Enemy>>& m_enemies;
	sf::RectangleShape m_waterStreamHitbox; // The visual and hitbox for the attack
	float m_stateDuration;                  // How long the attack lasts
};