// Kirby.h
#pragma once
#include "GameObj/MovingObj/MovingObject.h"
#include "GameObj/MovingObj/Enemy.h"
#include "PowerUpManager.h"
#include <memory>

class KirbyState;
class Wall;
class PowerUpCommand;

enum class PowerUpType { None, Spark };
enum class FacingDirection { Left = -1, Right = 1 };

class Kirby : public MovingObject
{
public:
	Kirby(const std::shared_ptr<sf::Texture>& kirbyTexture);

	// We override MovingObject's update to handle our new physics logic
	void update(float deltaTime) override;
	void move(float deltaTime) override {};

	//void move(float deltaTime) override;
	void attack(std::vector<std::unique_ptr<Enemy>>& enemies, float range);

	float getSpeed() const;
	void setSpeed(float speed);
	float getOriginalSpeed() const;

	// The single public method to add any PowerUp's effect
	void addPowerUpEffect(std::unique_ptr<PowerUpCommand> command);

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
	void heal(int healAmount);
	void takeDamage(int damageAmount);
	void loseLife();
	int getLives() const;
	int getHealth() const;
	int getMaxHealth() const;
	
	bool isHyper() const;
	void setHyper(bool hyper);
	bool isInvincible() const;

	// --- NEW POWER-UP FUNCTIONS ---
	void setPower(PowerUpType power);
	PowerUpType getCurrentPower() const;

	// We override draw to include state-specific effects
	void draw(sf::RenderTarget& target) const override;

	// --- NEW DIRECTION FUNCTIONS ---
	void setFacingDirection(FacingDirection dir);
	FacingDirection getFacingDirection() const;


private:
	void activateInvincibility(float deltaTime);

	std::unique_ptr<KirbyState> m_state;

	PowerUpManager m_PowerUpManager;
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

	PowerUpType m_currentPower = PowerUpType::None;
	FacingDirection m_facingDirection = FacingDirection::Right;
};