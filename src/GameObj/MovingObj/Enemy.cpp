// Enemy.cpp
#include "GameObj/MovingObj/Enemy.h"
#include "GameObjectFactory.h"
#include <SFML/Graphics.hpp>

sf::Color TwizzyColor(0, 0, 80); // Define Twizzy's color key

static bool isWaddleDeeRegistered = GameObjectFactory::instance().registerType(  
   sf::Color(0, 0, 40), // Color key for WaddleDee
   [](sf::Vector2f position) -> std::unique_ptr<GameObject> {  
	   auto enemyTexture = std::make_shared<sf::Texture>();
	   if (!enemyTexture->loadFromFile("WaddleDeeSprite.png")) {
		   throw std::runtime_error("Failed to load Waddle Dee texture");
	   }
       auto enemy = std::make_unique<Enemy>(enemyTexture, position);
       // enemy->setPosition(position);  
	   //enemy->setTexture(enemyTexture);
       return enemy;  
   }  
);  

static bool isTwizzyRegistered = GameObjectFactory::instance().registerType(  
   sf::Color(0, 0, 80),  // Color key for Twizzy
   [](sf::Vector2f position) -> std::unique_ptr<GameObject> {  
	   auto enemyTexture = std::make_shared<sf::Texture>();
	   if (!enemyTexture->loadFromFile("TwizzySprite.png")) {
		   throw std::runtime_error("Failed to load Twizzy texture");
	   }
       auto enemy = std::make_unique<Enemy>(enemyTexture, position);
       //enemy->setPosition(position);  
	   //enemy->setTexture(enemyTexture);
       return enemy;  
   }  
);  

static bool isGreenEnemyRegistered = GameObjectFactory::instance().registerType(  
   sf::Color(0, 0, 120), // Color key for Green Enemy  
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
	;
}


// Handle collision with walls by reversing direction
void Enemy::handleCollision(Wall* wall)
{
	m_direction = -m_direction;
	setPosition(m_oldPosition);
}
