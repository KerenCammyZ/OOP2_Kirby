// Enemy.h
#pragma once
#include "GameObj/MovingObj/MovingObject.h"
#include "GameObj/FixedObj/Wall.h"
#include <memory>

class Enemy : public MovingObject
{
public:
	Enemy(std::shared_ptr<sf::Texture>& enemyTexture, sf::Vector2f startPosition);

	// override
	void handleCollision(GameObject* other) {};
	void handleCollision(Kirby* kirby) {};
	void handleCollision(Door* door) {};
	void handleCollision(const Wall& wall) {};
	void move(float deltaTime) {};

private:

};

// virtual methods

/*
Inherits:
protected:
	sf::Vector2f m_position;
	sf::Vector2f m_size;
	std::shared_ptr<sf::Texture> m_texture;
	sf::Sprite m_sprite;
*/