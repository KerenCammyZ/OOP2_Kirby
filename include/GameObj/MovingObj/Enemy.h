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

	void handleCollision(GameObject* other) override {};
	void handleCollision(Kirby* kirby) override {};
	void handleCollision(Door* door) override {};
	void handleCollision(Wall* wall);

	sf::Sprite getSprite() const { return m_sprite; }

	//void setTexture(std::string filename);
private:
	sf::Vector2f m_direction{ -1.f, 0.f };
	sf::Texture m_texture;
	sf::Sprite m_sprite;
};