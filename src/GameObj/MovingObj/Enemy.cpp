// Enemy.cpp
#include <SFML/Graphics.hpp>
#include "GameObjectFactory.h"
#include "GameObj/MovingObj/Enemy.h"
#include "GameObj/MovingObj/Kirby.h"
#include "Behaviors/PatrolMove.h"
#include "Behaviors/FlyingMove.h"
#include "Behaviors/HoppingMove.h"
#include "Behaviors/SimpleAttack.h"
#include "Behaviors/SparkAttack.h"
#include "Behaviors/IgnoreWalls.h"

sf::Color WaddleDeeColor(0, 0, 40); // Define Waddle Dee's color key
sf::Color TwizzyColor(0, 0, 80); // Define Twizzy's color key
sf::Color sparkyColor(0, 0, 120);

// Static registration for WaddleDee
bool Enemy::m_registerWaddleDee = GameObjectFactory::registerType(
	WaddleDeeColor, [](sf::Vector2f position, const Kirby* kirby) -> std::unique_ptr<GameObject> {
		auto enemyTexture = std::make_shared<sf::Texture>();
		if (!enemyTexture->loadFromFile("WaddleDeeSprite.png")) {
			throw std::runtime_error("Failed to load Waddle Dee texture");
		}
		auto enemy = std::make_unique<Enemy>(enemyTexture, position, kirby);
		enemy->setMoveBehavior(std::make_unique<PatrolMove>());
		enemy->setAttackBehavior(std::make_unique<SimpleAttack>());
		enemy->setDirection(sf::Vector2f(-1.f, 0.f));
		enemy->setSpeed(100.0f); // Set a slower speed for Waddle Dee
		enemy->setDamageAmount(1); // Set damage amount for Waddle Dee
		enemy->setScoreValue(10); // number of points awarded for defeating Waddle Dee
		enemy->setGrounded(true);
		return enemy;
	}  
);  

// Static registration for Twizzy 
bool Enemy::m_registerTwizzy = GameObjectFactory::registerType(
	TwizzyColor, [](sf::Vector2f position, const Kirby* kirby) -> std::unique_ptr<GameObject> {
		auto enemyTexture = std::make_shared<sf::Texture>();
		if (!enemyTexture->loadFromFile("TwizzySprite.png")) {
			throw std::runtime_error("Failed to load Twizzy texture");
		}
		auto enemy = std::make_unique<Enemy>(enemyTexture, position, kirby);
		enemy->setMoveBehavior(std::make_unique<FlyingMove>());
		enemy->setAttackBehavior(std::make_unique<SimpleAttack>());
		enemy->setCollisionBehavior(std::make_unique<IgnoreWalls>());
		enemy->setDirection(sf::Vector2f(-1.f, 0.f));
		enemy->setDamageAmount(1); // Set damage amount for Twizzy
		enemy->setScoreValue(25);
		enemy->setGrounded(false);
		return enemy;  
	}  
);  

// Static registration for Sparky
bool Enemy::m_registerSparky = GameObjectFactory::registerType(
	sparkyColor,
	[](sf::Vector2f position, const Kirby* kirby) -> std::unique_ptr<GameObject> {
		auto enemyTexture = std::make_shared<sf::Texture>();
		if (!enemyTexture->loadFromFile("SparkySprite.png")) {
			throw std::runtime_error("Failed to load Sparky texture");
		}
		auto enemy = std::make_unique<Enemy>(enemyTexture, position, kirby);
		enemy->setMoveBehavior(std::make_unique<HoppingMove>());
		enemy->setAttackBehavior(std::make_unique<SparkAttack>());
		enemy->setDirection(sf::Vector2f(-1.f, 0.f));
		enemy->setDamageAmount(2); // Set damage amount for Sparky
		enemy->setScoreValue(200);
		enemy->setGrounded(false); // this will change in Hopping Behavior
		return enemy;
	}
);


Enemy::Enemy(const std::shared_ptr<sf::Texture>& enemyTexture, sf::Vector2f startPosition, const Kirby* kirby)
	: m_state(EnemyState::SPAWNING),
	m_kirby(kirby) // Initialize the new member
{
	setTexture(enemyTexture);
	setPosition(startPosition);
	setSize(sf::Vector2f(ENTITY_SIZE, ENTITY_SIZE));
	m_speed = 180.0f;

	m_actionTimer = 2.0f + (rand() % 3);
}

Enemy::~Enemy()
{
	// Clean up behaviors
	if (m_attackBehavior) m_attackBehavior->setOwner(nullptr);
	if (m_moveBehavior) m_moveBehavior->setOwner(nullptr);
	if (m_collisionBehavior) m_collisionBehavior->setOwner(nullptr);
}



//void Enemy::update(float deltaTime)
//{
//	// Hanlde state transitions first
//	if (m_state == EnemyState::SPAWNING)
//	{
//		// Handle spawning state logic
//		m_spawnTimer -= deltaTime;
//		if (m_spawnTimer <= 0.0f)
//		{
//			m_state = EnemyState::ACTIVE;
//		}
//		GameObject::update(deltaTime);
//		return;
//	}
//	else if (m_state == EnemyState::STUNNED)
//	{
//		// Handle stunned state logic
//		m_stunTimer -= deltaTime;
//		if (m_stunTimer <= 0.0f)
//		{
//			m_state = EnemyState::ACTIVE;
//		}
//		GameObject::update(deltaTime);
//		return;
//	}
//	if (m_state == EnemyState::SWALLOWED)
//	{
//		// removal is handled by the GameController
//		// GameController::update(float deltaTime)
//
//		;
//	}
//	if (m_state == EnemyState::ACTIVE)
//	{
//		// Handle active state logic
//		m_oldPosition = m_position;
//		move(deltaTime);
//		//attack(deltaTime);
//		GameObject::update(deltaTime);
//		return;
//		
//	}
//
//	if(m_state == EnemyState::ATTACKING)
//	{
//		// Handle attacking state logic
//		//m_attackTimer -= deltaTime;
//		//attack(deltaTime);
//		//if (m_attackTimer <= 0.0f)
//		//{
//		//	m_state = EnemyState::ACTIVE; // Return to active state after attack
//		//}
//		return;
//	}
//
//	GameObject::update(deltaTime);
//}

void Enemy::update(float deltaTime)
{
	// Handle non-active states first
	if (m_state == EnemyState::SPAWNING)
	{
		m_spawnTimer -= deltaTime;
		if (m_spawnTimer <= 0.0f) {
			m_state = EnemyState::ACTIVE;
		}
		GameObject::update(deltaTime);
		return;
	}
	else if (m_state == EnemyState::STUNNED || m_state == EnemyState::SWALLOWED)
	{
		// No updates needed for these states, just let them exist
		GameObject::update(deltaTime);
		return;
	}

	// --- State Logic for Active and Attacking ---

	if (m_state == EnemyState::ACTIVE)
	{
		// Countdown the timer until the next attack
		m_actionTimer -= deltaTime;

		if (m_actionTimer <= 0.0f && isGrounded())
		{
			// Timer is up, switch to ATTACKING state
			m_state = EnemyState::ATTACKING;
			m_attackDuration = 1.5f; // Attack will last for 1.5 seconds
		}
		else
		{
			// If not attacking, move normally
			m_oldPosition = m_position;
			move(deltaTime);
		}
	}
	else if (m_state == EnemyState::ATTACKING)
	{
		// When attacking, the enemy does NOT move.
		// Countdown the duration of the attack.
		m_attackDuration -= deltaTime;

		if (m_attackDuration <= 0.0f)
		{
			// Attack is over, switch back to ACTIVE state
			m_state = EnemyState::ACTIVE;
			m_actionTimer = 2.0f + (rand() % 3); // Reset timer for the next attack cycle
		}
		else
		{
			// While the attack is happening, call the attack behavior
			attack(deltaTime);
		}
	}

	GameObject::update(deltaTime);
}

// Stun the enemy for a specified duration
void Enemy::stun(float duration)
{
	m_state = EnemyState::STUNNED;
	m_stunTimer = duration;
}

// Handle the enemy being swallowed by Kirby
void Enemy::onSwallowed()
{
	m_sprite.setColor(sf::Color::Red);
	m_state = EnemyState::SWALLOWED;
	std::cout << "Enemy swallowed!" << std::endl;
	// remove enemy from the game world
}

// Reverse the enemy's direction
// by flipping the sprite horizontally and reversing the direction vector
void Enemy::reverseDirection()
{
	sf::Vector2f currentScale = m_sprite.getScale();
	m_sprite.setScale(-currentScale.x, currentScale.y);
	m_direction.x = -m_direction.x;
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
			// Handle collision by reversing direction
			reverseDirection();
			setPosition(m_oldPosition);
		}
		else if (other->getType() == ObjectType::ENEMY)
		{
			reverseDirection();
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

void Enemy::setGrounded(bool grounded)
{
	m_isGrounded = grounded;
}

bool Enemy::isGrounded() const
{
	return m_isGrounded;
}