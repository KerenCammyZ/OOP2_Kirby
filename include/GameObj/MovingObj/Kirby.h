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

	void update(float deltaTime) override;
	void move(float deltaTime) override {};
	void move(float deltaTime, const std::vector<std::unique_ptr<GameObject>>& obstacles);
	
	void attack(std::vector<std::unique_ptr<Enemy>>& enemies, float range);
	void draw(sf::RenderTarget& target) const override;

	float getSpeed() const;
	void setSpeed(float speed);
	float getOriginalSpeed() const;

	void addPowerUpEffect(std::unique_ptr<PowerUpCommand> command);

	void setVelocity(const sf::Vector2f& velocity);
	sf::Vector2f getVelocity() const;

	void setGrounded(bool grounded);
	bool isGrounded() const;

	void setInWater(bool inWater);
	bool isInWater() const;

	void updateGroundSensor();
	sf::FloatRect getGroundSensorBounds() { return m_groundSensor.getGlobalBounds(); }

	// Collision Handlers
	void handleCollision(GameObject* other) override;
	void handleCollision(Door* door);
	void handleCollision(Spike* spike) override;

	// Kirby's health and lives management
	void heal(int healAmount);
	void takeDamage(int damageAmount);
	void loseLife();
	void addLife(int lifeAmount);
	void setLife(int life);
	void setHealth(int health);
	int getLives() const;
	int getHealth() const;
	int getMaxHealth() const;
	
	bool isHyper() const;
	void setHyper(bool hyper);
	bool isInvincible() const;

	void setPower(PowerUpType power);
	PowerUpType getCurrentPower() const;

	
	void setState(std::unique_ptr<KirbyState> state);
	void setAnimation(const std::string& name);

	void setFacingDirection(FacingDirection dir);
	FacingDirection getFacingDirection() const;
	bool isFacingLeft() const { return m_facingLeft; }
	void setFacingLeft(bool facingLeft) { m_facingLeft = facingLeft; }
	bool isMovingHorizontally() const { return std::abs(m_velocity.x) > 0.1f; }


private:
	void setupAnimations();
	void updateAnimation(float deltaTime);
	void activateInvincibility(float deltaTime);
	bool willCollideAt(const sf::Vector2f& testPosition,
		const std::vector<std::unique_ptr<GameObject>>& obstacles) const;


	PowerUpManager m_PowerUpManager;
	std::unique_ptr<KirbyState> m_state;

	float m_animationChangeDelay;
	std::string m_lastAnimationState;
	std::unique_ptr<Animator> m_animator;
	std::shared_ptr<SpriteSheet> m_spriteSheet;

	float m_originalSpeed;
	sf::Vector2f m_velocity;

	int m_lives = 5;
	int m_health = 6;
	int m_maxHealth = 6;
	bool m_inWater = false;
	bool m_isHyper = false;
	bool m_isInvincible = false;
	float m_invincibilityTimer = 0.0f; // Prevents rapid damage
	bool m_isGrounded;
	sf::RectangleShape m_groundSensor; // Detect ground collision
	const std::vector<std::unique_ptr<GameObject>>* m_obstacles = nullptr;

	PowerUpType m_currentPower = PowerUpType::None;
	bool m_facingLeft = false;
	FacingDirection m_facingDirection = FacingDirection::Right;
};