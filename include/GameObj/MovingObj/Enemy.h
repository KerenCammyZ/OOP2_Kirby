// Enemy.h
#pragma once
#include "GameObj/MovingObj/MovingObject.h"
#include "GameObj/FixedObj/Wall.h"

class Enemy : public MovingObject
{
public:
	Enemy();
	Enemy(std::shared_ptr<sf::Texture>& enemyTexture, sf::Vector2f startPosition);
	void move(float deltaTime) override;

	void handleCollision(GameObject* other) {};
	void handleCollision(Kirby* kirby) {};
	void handleCollision(Door* door) {};
	void handleCollision(Wall* wall);

private:
	sf::Vector2f m_direction{ -1.f, 0.f };
};