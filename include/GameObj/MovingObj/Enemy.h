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
	void update(float deltaTime) override;
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

private:
	std::string name; // for debugging

	std::unique_ptr <MoveBehavior> m_moveBehavior;
	std::unique_ptr <AttackBehavior> m_attackBehavior;

	sf::Vector2f m_direction{ -1.f, 0.f };
	sf::Texture m_texture;
	sf::Sprite m_sprite;
};