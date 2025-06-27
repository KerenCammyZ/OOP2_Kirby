// Enemy.cpp
#include <SFML/Graphics.hpp>
#include "GameObjectFactory.h"
#include "GameObj/MovingObj/Enemy.h"
#include "GameObj/MovingObj/Kirby.h"
#include "GameObj/Behaviors/PatrolMove.h"
#include "GameObj/Behaviors/FlyingMove.h"
#include "GameObj/Behaviors/SimpleAttack.h"
#include "GameObj/Behaviors/IgnoreWalls.h"

sf::Color WaddleDeeColor(0, 0, 40); // Define Waddle Dee's color key
sf::Color TwizzyColor(0, 0, 80); // Define Twizzy's color key

// Static registration for WaddleDee
bool Enemy::m_registerWaddleDee = GameObjectFactory::registerType(
	WaddleDeeColor, [](sf::Vector2f position) -> std::unique_ptr<GameObject> {
		auto enemyTexture = std::make_shared<sf::Texture>();
		if (!enemyTexture->loadFromFile("WaddleDeeSprite.png")) {
			throw std::runtime_error("Failed to load Waddle Dee texture");
		}
		auto enemy = std::make_unique<Enemy>(enemyTexture, position);
		enemy->setMoveBehavior(std::make_unique<PatrolMove>());
		enemy->setAttackBehavior(std::make_unique<SimpleAttack>());
		enemy->setDirection(sf::Vector2f(-1.f, 0.f));
		enemy->setSpeed(100.0f); // Set a slower speed for Waddle Dee
		enemy->setDamageAmount(1); // Set damage amount for Waddle Dee
		return enemy;
	}  
);  

// Static registration for Twizzy 
bool Enemy::m_registerTwizzy = GameObjectFactory::registerType(
	TwizzyColor, [](sf::Vector2f position) -> std::unique_ptr<GameObject> {  
		auto enemyTexture = std::make_shared<sf::Texture>();
		if (!enemyTexture->loadFromFile("TwizzySprite.png")) {
			throw std::runtime_error("Failed to load Twizzy texture");
		}
		auto enemy = std::make_unique<Enemy>(enemyTexture, position);
		enemy->setMoveBehavior(std::make_unique<FlyingMove>());
		enemy->setAttackBehavior(std::make_unique<SimpleAttack>());
		enemy->setCollisionBehavior(std::make_unique<IgnoreWalls>());
		enemy->setDirection(sf::Vector2f(-1.f, 0.f));
		enemy->setDamageAmount(3); // Set damage amount for Twizzy
		return enemy;  
	}  
);  

//bool Enemy::isGreenEnemyRegistered = GameObjectFactory::registerType(  
//   sf::Color(0, 0, 160), // Color key for Green Enemy  
//   [](sf::Vector2f position) -> std::unique_ptr<GameObject> {  
//       auto enemy = std::make_unique<Enemy>();  
//       enemy->setPosition(position);  
//       // Optionally set size, texture, etc.  
//       return enemy;  
//   }  
//);

Enemy::Enemy() = default; // Default constructor

Enemy::Enemy(std::shared_ptr<sf::Texture>& enemyTexture, sf::Vector2f startPosition)
	: m_state(EnemyState::SPAWNING), m_direction(sf::Vector2f(0.f, 0.f))
{
	setTexture(enemyTexture); // initialize m_texture, m_sprite
	setPosition(startPosition);
	setSize(sf::Vector2f(ENTITY_SIZE, ENTITY_SIZE));
	m_speed = 180.0f;
}


void Enemy::update(float deltaTime)
{
	// Hanlde state transitions first
	if (m_state == EnemyState::SPAWNING)
	{
		// Handle spawning state logic
		m_spawnTimer -= deltaTime;
		if (m_spawnTimer <= 0.0f)
		{
			m_state = EnemyState::ACTIVE;
		}
		GameObject::update(deltaTime);
		return;
	}
	if (m_state == EnemyState::STUNNED)
	{
		// Handle stunned state logic
		m_stunTimer -= deltaTime;
		if (m_stunTimer <= 0.0f)
		{
			m_state = EnemyState::ACTIVE;
		}
		GameObject::update(deltaTime);
		return;
	}
	if (m_state == EnemyState::ACTIVE)
	{
		// Handle active state logic
		m_oldPosition = m_position;
		move(deltaTime);
		//attack(deltaTime);
		GameObject::update(deltaTime);
		return;
	}

	GameObject::update(deltaTime);
}

// Stun the enemy for a specified duration
void Enemy::stun(float duration)
{
	m_state = EnemyState::STUNNED;
	m_stunTimer = duration;
}

// Handle collision with Kirby
void Enemy::handleCollision(Kirby* kirby)
{
	stun(1.0f); // Stun the enemy for 1 second on collision with Kirby
	if (!kirby->isInvincible())
	{
		kirby->takeDamage(m_damageAmount);
	}
}

// Handle collision with other game objects
void Enemy::handleCollision(GameObject* other)
{
	if (m_collisionBehavior) { // owner has special collision behavior
		m_collisionBehavior->handleCollision(other);
		return;
	}
	else { // Default collision handling
		if (other->getType() == ObjectType::WALL)
		{
			// Handle collision with walls by reversing direction
			sf::Vector2f currentScale = m_sprite.getScale();
			m_sprite.setScale(-currentScale.x, currentScale.y);
			m_direction = -m_direction; // Reverse direction on collision
			setPosition(m_oldPosition);
		}
		else if (other->getType() == ObjectType::ENEMY)
		{
			// Handle collision by reversing direction
			sf::Vector2f currentScale = m_sprite.getScale();
			m_sprite.setScale(-currentScale.x, currentScale.y);
			m_direction = -m_direction; // Reverse direction on collision
			setPosition(m_oldPosition);
		}
	}
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
	moveBehavior->setOwner(this);
	m_moveBehavior = std::move(moveBehavior);
}

void Enemy::setAttackBehavior(std::unique_ptr<AttackBehavior> attackBehavior)
{
	attackBehavior->setOwner(this);
	m_attackBehavior = std::move(attackBehavior);
}

void Enemy::setCollisionBehavior(std::unique_ptr<CollisionBehavior> collisionBehavior)
{
	if (collisionBehavior)
	{
		collisionBehavior->setOwner(this);
	}
	m_collisionBehavior = std::move(collisionBehavior);
}
