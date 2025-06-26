// Enemy.cpp
#include <SFML/Graphics.hpp>
#include "GameObjectFactory.h"
#include "GameObj/MovingObj/Enemy.h"
#include "GameObj/Behaviors/PatrolMove.h"
#include "GameObj/Behaviors/FlyingMove.h"
#include "GameObj/Behaviors/SimpleAttack.h"

sf::Color TwizzyColor(0, 0, 80); // Define Twizzy's color key

static bool isWaddleDeeRegistered = GameObjectFactory::instance().registerType(  
   sf::Color(0, 0, 40), // Color key for WaddleDee
   [](sf::Vector2f position) -> std::unique_ptr<GameObject> {  
	   auto enemyTexture = std::make_shared<sf::Texture>();
	   if (!enemyTexture->loadFromFile("WaddleDeeSprite.png")) {
		   throw std::runtime_error("Failed to load Waddle Dee texture");
	   }
       auto enemy = std::make_unique<Enemy>(enemyTexture, position);
	   enemy->setMoveBehavior(std::make_unique<PatrolMove>());
	   enemy->setAttackBehavior(std::make_unique<SimpleAttack>());
	   enemy->setName("WaddleDee"); // Set type for debugging
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
	   enemy->setMoveBehavior(std::make_unique<FlyingMove>());
	   enemy->setAttackBehavior(std::make_unique<SimpleAttack>());
	   enemy->setName("Twizzy"); // Set type for debugging
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

void Enemy::update(float deltaTime)
{
	std::cout << getName() << ":\n";
	move(deltaTime);
	attack(deltaTime);
	std::cout << "\n";
}

// Handle collision with walls by reversing direction
void Enemy::handleCollision(Wall* wall)
{
	m_direction = -m_direction;
	setPosition(m_oldPosition);
}

void Enemy::move(float deltaTime)
{
	if (m_moveBehavior)
		m_moveBehavior->move(deltaTime);
}

void Enemy::attack(float deltaTime)
{
	if (m_attackBehavior)
		m_attackBehavior->attack(deltaTime);
}

void Enemy::setMoveBehavior(std::unique_ptr<MoveBehavior> moveBehavior)
{
	m_moveBehavior = std::move(moveBehavior);
}

void Enemy::setAttackBehavior(std::unique_ptr<AttackBehavior> attackBehavior)
{
	m_attackBehavior = std::move(attackBehavior);
}