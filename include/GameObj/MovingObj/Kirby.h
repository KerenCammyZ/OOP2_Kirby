// Kirby.h
#pragma once
#include "GameObj/MovingObj/MovingObject.h"
#include "GameObj/MovingObj/Enemy.h"
#include "PowerUpManager.h"
#include "SpriteSheet.h"
#include "Animator.h"
#include <memory>
#include <vector>


class KirbyState;
class Wall;
class PowerUpCommand;
class Spike;

enum class PowerUpType { None, Spark };
enum class FacingDirection { Left = -1, Right = 1 };

class Kirby : public MovingObject
{
public:
	Kirby(const std::shared_ptr<sf::Texture>& kirbyTexture);

	// We override MovingObject's update to handle our new physics logic
	void update(float deltaTime) override;
	void move(float deltaTime) override {};
	void move(float deltaTime, const std::vector<std::unique_ptr<GameObject>>& obstacles);
	
	void attack(std::vector<std::unique_ptr<Enemy>>& enemies, float range);

	void draw(sf::RenderTarget& target) const override;   // TODO: Resolve Merge
	// We override draw to include state-specific effects
	//void draw(sf::RenderTarget& target) const override;

	float getSpeed() const;
	void setSpeed(float speed);
	float getOriginalSpeed() const;
	sf::FloatRect getGroundSensorBounds() { return m_groundSensor.getGlobalBounds(); }

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
	void updateGroundSensor();
	void handleCollision(Door* door) override;
	void handleCollision(Spike* spike) override;

	// Kirby's health and lives management
	void heal(int healAmount);
	void takeDamage(int damageAmount);
	void loseLife();
	void addLife(int lifeAmount);
	int getLives() const;
	int getHealth() const;
	int getMaxHealth() const;
	
	bool isHyper() const;
	void setHyper(bool hyper);
	bool isInvincible() const;

	// --- NEW POWER-UP FUNCTIONS ---
	void setPower(PowerUpType power);
	PowerUpType getCurrentPower() const;


	// --- NEW DIRECTION FUNCTIONS ---
	void setFacingDirection(FacingDirection dir);
	FacingDirection getFacingDirection() const;
	

	void setState(std::unique_ptr<KirbyState> state);
	void setAnimation(const std::string& name);


	bool isFacingLeft() const { return m_facingLeft; }
	void setFacingLeft(bool facingLeft) { m_facingLeft = facingLeft; }
	bool isMovingHorizontally() const { return std::abs(m_velocity.x) > 0.1f; }


private:
	void activateInvincibility(float deltaTime);
	bool willCollideAt(const sf::Vector2f& testPosition, // TODO: change function name to CheckCollisionAt
		const std::vector<std::unique_ptr<GameObject>>& obstacles) const;

	std::unique_ptr<Animator> m_animator;
	std::string m_lastAnimationState;
	float m_animationChangeDelay;

	void setupAnimations();
	void updateAnimation(float deltaTime);

	std::unique_ptr<KirbyState> m_state;

	PowerUpManager m_PowerUpManager;
	float m_originalSpeed;

	// Animation system
	std::shared_ptr<SpriteSheet> m_spriteSheet;

	// --- NEW PHYSICS MEMBERS ---
	sf::Vector2f m_velocity;
	bool m_isGrounded;

	bool m_inWater = false;
	bool m_isHyper = false;
	bool m_isInvincible = false;
	bool m_facingLeft = false;
	int m_lives = 5;
	int m_health = 6;
	int m_maxHealth = 6;
	float m_invincibilityTimer = 0.0f; // Prevents rapid damage
	sf::RectangleShape m_groundSensor; //will be used to detect ground collision
	const std::vector<std::unique_ptr<GameObject>>* m_obstacles = nullptr;

	PowerUpType m_currentPower = PowerUpType::None;
	FacingDirection m_facingDirection = FacingDirection::Right;
};