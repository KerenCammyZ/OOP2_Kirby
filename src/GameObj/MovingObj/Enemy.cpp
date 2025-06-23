// Enemy.cpp
#include "GameObj/MovingObj/Enemy.h"
#include <SFML/Graphics.hpp>

Enemy::Enemy(std::shared_ptr<sf::Texture>& enemyTexture, sf::Vector2f startPosition)
	//: m_texture(enemyTexture), m_position(startPosition)
{
	setTexture(enemyTexture); // initialize m_texture, m_sprite
	setPosition(startPosition);
	sf::Vector2f enemySize(ENTITY_SIZE, ENTITY_SIZE);
	setSize(enemySize);

	// Initialize any additional enemy-specific properties here
}
