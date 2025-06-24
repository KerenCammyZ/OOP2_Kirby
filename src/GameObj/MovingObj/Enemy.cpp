// Enemy.cpp
#include "GameObj/MovingObj/Enemy.h"
#include "GameObjectFactory.h"
#include <SFML/Graphics.hpp>

// Example: Register enemy with a specific color (e.g., green)
static bool isEnemyRegistered = GameObjectFactory::instance().registerType(
	sf::Color(0, 255, 0), // Color key for Enemy
	[](sf::Vector2f position) -> std::unique_ptr<GameObject> {
		auto enemy = std::make_unique<Enemy>();
		enemy->setPosition(position);
		// Optionally set size, texture, etc.
		return enemy;
	}
);

Enemy::Enemy()
{
}

Enemy::Enemy(std::shared_ptr<sf::Texture>& enemyTexture, sf::Vector2f startPosition)
	//: m_texture(enemyTexture), m_position(startPosition)
{
	setTexture(enemyTexture); // initialize m_texture, m_sprite
	setPosition(startPosition);
	setSize(sf::Vector2f(ENTITY_SIZE, ENTITY_SIZE));
	m_speed = 180.0f;
}


void Enemy::move(float deltaTime)
{
	m_position += m_direction * m_speed * deltaTime;
}

// Handle collision with walls by reversing direction
void Enemy::handleCollision(Wall* wall)
{
	setPosition(m_oldPosition);
	m_direction = -m_direction;
}