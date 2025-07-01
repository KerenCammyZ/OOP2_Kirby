// Kirby.h
#pragma once
#include "GameObj/MovingObj/MovingObject.h"
#include "GameObj/MovingObj/Enemy.h"
#include "PresentManager.h"
#include <memory>

class KirbyState;
class Wall;
class PresentCommand;

class Kirby : public MovingObject
{
public:
	Kirby(std::shared_ptr<sf::Texture>& kirbyTexture);

	// We override MovingObject's update to handle our new physics logic
	void update(float deltaTime) override;
	void move(float deltaTime) override {};

	//void move(float deltaTime) override;
	void attack(std::vector<std::unique_ptr<Enemy>>& enemies, float range);

	float getSpeed() const;
	void setSpeed(float speed);
	float getOriginalSpeed() const;
	
	// The single public method to add any present's effect
	void addPresentEffect(std::unique_ptr<PresentCommand> command);

	// --- NEW PHYSICS GETTERS/SETTERS ---
	void setVelocity(const sf::Vector2f& velocity);
	sf::Vector2f getVelocity() const;

	void setGrounded(bool grounded);
	bool isGrounded() const;

	// --- NEW: Water state management ---
	void setInWater(bool inWater);
	bool isInWater() const;

	// Collision Handlers
	void handleCollision(GameObject* other) override;
	void handleCollision(Kirby* kirby) override {};
	void handleCollision(Door* door) override;

	// Kirby's health and lives management
	void takeDamage(int damageAmount);
	void heal(int healAmount);
	void loseLife();
	int getHealth() const { return m_health; }
	int getMaxHealth() const { return m_maxHealth; }
	int getLives() const { return m_lives; }
	bool isInvincible() const { return m_isInvincible; }

	bool isHyper() const { return m_isHyper; }
	void setHyper(bool hyper) { m_isHyper = hyper; }

private:
	std::unique_ptr<KirbyState> m_state;

	PresentManager m_presentManager;
	float m_originalSpeed;

	// --- NEW PHYSICS MEMBERS ---
	sf::Vector2f m_velocity;
	bool m_isGrounded;

	int m_health = 6;
	int m_maxHealth = 6;
	int m_lives = 5;
	bool m_isInvincible = false;
	bool m_isHyper = false;
	float m_invincibilityTimer = 0.0f; // Prevents rapid damage
	bool m_inWater = false;
};