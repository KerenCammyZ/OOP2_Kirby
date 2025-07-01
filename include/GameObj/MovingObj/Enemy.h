// Enemy.h
#pragma once
#include <memory>
#include "GameObj/MovingObj/MovingObject.h"
#include "GameObj/FixedObj/Wall.h"
#include "Behaviors/MoveBehavior.h"
#include "Behaviors/AttackBehavior.h"
#include "Behaviors/CollisionBehavior.h"

class Kirby;
enum class EnemyState { SPAWNING, ACTIVE, SWALLOWED, STUNNED, ATTACKING };

class Enemy : public MovingObject
{
public:
	Enemy() = default;
	//Enemy(std::shared_ptr<sf::Texture>& enemyTexture, sf::Vector2f startPosition);
	Enemy(std::shared_ptr<sf::Texture>& enemyTexture, sf::Vector2f startPosition, const Kirby* kirby);

	void update(float deltaTime) override;
	void move(float deltaTime) override;
	void attack(float deltaTime);
	void stun(float duration);
	void onSwallowed();
	void reverseDirection();
	
	void handleCollision(GameObject* other) override;
	void handleCollision(Kirby* kirby) override;
	void handleCollision(Door* door) override {};

	// Setters for strategies
	void setMoveBehavior(std::unique_ptr<MoveBehavior> moveBehavior);
	void setAttackBehavior(std::unique_ptr<AttackBehavior> attackBehavior);
	void setCollisionBehavior(std::unique_ptr<CollisionBehavior> collisionBehavior);
	
	void setDamageAmount(int damage) { m_damageAmount = damage; }
	void setScoreValue(int score) { m_scoreValue = score; }
	bool isSwallowed() const { return m_state == EnemyState::SWALLOWED; }

	ObjectType getType() const { return ObjectType::ENEMY; }
	int getScoreValue() const { return m_scoreValue; }

private:
	std::unique_ptr <MoveBehavior> m_moveBehavior;
	std::unique_ptr <AttackBehavior> m_attackBehavior;
	std::unique_ptr <CollisionBehavior> m_collisionBehavior;

	const Kirby* m_kirby;

	EnemyState m_state;

	// Timers for State Management
	int m_damageAmount;
	int m_scoreValue = 10; // Score value for defeating this enemy
	float m_stunTimer{ 0.0f };
	float m_spawnTimer{ 1.0f };
	float m_actionTimer{ 0.0f };
	float m_attackDuration{ 0.0f };


	// Static registration for the Enemy type
	static bool m_registerTwizzy;
	static bool m_registerWaddleDee;
	static bool m_registerSparky;
	//static bool m_registerEnemy;
};