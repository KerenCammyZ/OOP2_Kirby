// Enemy.h
#pragma once
#include <memory>
#include "GameObj/MovingObj/MovingObject.h"
#include "GameObj/FixedObj/Wall.h"

#include "GameObj/Behaviors/MoveBehavior.h"
#include "GameObj/Behaviors/AttackBehavior.h"

class Enemy : public MovingObject
{
public:
	Enemy();
	Enemy(std::shared_ptr<sf::Texture>& enemyTexture, sf::Vector2f startPosition);
	//void update(float deltaTime) override;
	void move(float deltaTime) override;
	void attack(float deltaTime);

	void handleCollision(GameObject* other) override {};
	void handleCollision(Kirby* kirby) override {};
	void handleCollision(Door* door) override {};
	void handleCollision(Wall* wall);

	// Setters for strategies
	void setMoveBehavior(std::unique_ptr<MoveBehavior> moveBehavior);
	void setAttackBehavior(std::unique_ptr<AttackBehavior> attackBehavior);
	
	// for debugging
	std::string getName() const { return name; }
	void setName(const std::string& type) { name = type; }
	bool hasTexture() { return (m_texture != nullptr); }
	sf::Vector2f getTextureSize() { return sf::Vector2f(m_texture->getSize()); }
	sf::Sprite getSprite() const { return m_sprite; }
	sf::Vector2f getpritePosition() { return m_sprite.getPosition(); }

	ObjectType getType() const { return ObjectType::ENEMY; }
	sf::Vector2f getDirection() const { return m_direction; }

private:
	std::string name; // for debugging

	std::unique_ptr <MoveBehavior> m_moveBehavior;
	std::unique_ptr <AttackBehavior> m_attackBehavior;

	sf::Vector2f m_direction{ -1.f, 0.f };
	static bool m_registeritem; // Static member to register this type with the factory
	//static bool m_registerTwizzy;
	//static bool m_registerWaddleDee;
	static bool isGreenEnemyRegistered;
	static bool isTwizzyRegistered;
	static bool isWaddleDeeRegistered;
};