// Enemy.h
#pragma once
#include <memory>
#include "GameObj/MovingObj/MovingObject.h"
#include "GameObj/FixedObj/Wall.h"
#include "Behaviors/MoveBehavior.h"
#include "Behaviors/AttackBehavior.h"
#include "Behaviors/CollisionBehavior.h"

enum class EnemyState { SPAWNING, ACTIVE, STUNNED, SWALLOWED };

class Enemy : public MovingObject
{
public:
	Enemy();
	Enemy(std::shared_ptr<sf::Texture>& enemyTexture, sf::Vector2f startPosition);

	void update(float deltaTime) override;
	void move(float deltaTime) override;
	void attack(float deltaTime);
	void stun(float duration);
	void onSwallowed();
	
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

	EnemyState m_state;
	float m_stunTimer{ 0.0f }; // enemy stunned
	float m_spawnTimer{ 1.25f };
	int m_damageAmount;
	int m_scoreValue; // Score value for defeating this enemy

	// Static registration for the Enemy type
	static bool m_registerTwizzy;
	static bool m_registerWaddleDee;
	//static bool m_registerEnemy;
};