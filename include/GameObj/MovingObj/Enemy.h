// Enemy.h
#pragma once
#include <memory>
#include "GameObj/MovingObj/MovingObject.h"
#include "GameObj/FixedObj/Wall.h"
#include "GameObj/Behaviors/MoveBehavior.h"
#include "GameObj/Behaviors/AttackBehavior.h"
#include "GameObj/Behaviors/CollisionBehavior.h"

enum class EnemyState { SPAWNING, ACTIVE, STUNNED };

class Enemy : public MovingObject
{
public:
	Enemy();
	Enemy(std::shared_ptr<sf::Texture>& enemyTexture, sf::Vector2f startPosition);

	void update(float deltaTime) override;
	void move(float deltaTime) override;
	void attack(float deltaTime);
	void stun(float duration);

	void handleCollision(GameObject* other) override;
	void handleCollision(Kirby* kirby) override;
	void handleCollision(Door* door) override {};

	// Setters for strategies
	void setMoveBehavior(std::unique_ptr<MoveBehavior> moveBehavior);
	void setAttackBehavior(std::unique_ptr<AttackBehavior> attackBehavior);
	void setCollisionBehavior(std::unique_ptr<CollisionBehavior> collisionBehavior);
	
	void setDirection(const sf::Vector2f& direction) { m_direction = direction; }
	

	// for debugging
	//std::string name; // for debugging
	//std::string getName() const { return name; }
	//void setName(const std::string& type) { name = type; }
	//bool hasTexture() { return (m_texture != nullptr); }
	//sf::Vector2f getTextureSize() { return sf::Vector2f(m_texture->getSize()); }
	//sf::Sprite getSprite() const { return m_sprite; }
	//sf::Vector2f getpritePosition() { return m_sprite.getPosition(); }

	ObjectType getType() const { return ObjectType::ENEMY; }
	sf::Vector2f getDirection() const { return m_direction; }

private:
	std::unique_ptr <MoveBehavior> m_moveBehavior;
	std::unique_ptr <AttackBehavior> m_attackBehavior;
	std::unique_ptr <CollisionBehavior> m_collisionBehavior;

	EnemyState m_state;
	sf::Vector2f m_direction;
	float m_stunTimer{ 0.0f }; // enemy stunned
	float m_spawnTimer{ 1.25f };

	// Static registration for the Enemy type
	static bool m_registerTwizzy;
	static bool m_registerWaddleDee;
	//static bool m_registerEnemy;
};