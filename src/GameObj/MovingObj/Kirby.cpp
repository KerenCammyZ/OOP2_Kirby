#include "GameObj/MovingObj/Kirby.h"
#include "GameObj/MovingObj/Enemy.h"
#include "GameObj/FixedObj/Door.h"
#include "States/KirbyStates/KirbyStandingState.h"
#include "States/KirbyStates/KirbySwimmingState.h"
#include "GlobalSizes.h"
#include "GameObj/FixedObj/Wall.h"
#include "States/KirbyStates/KirbySparkAttackState.h"
#include "States/KirbyStates/KirbyWalkingState.h" 
#include <iostream>
#include <thread>
#include <chrono>

Kirby::Kirby(const std::shared_ptr<sf::Texture>& kirbyTexture)
	: m_velocity(0.f, 0.f), m_isGrounded(false), m_inWater(false) // Initialize physics members
{
	setTexture(kirbyTexture);
	sf::Vector2f kirbySize(ENTITY_SIZE, ENTITY_SIZE);
	setSize(kirbySize);
	sf::Vector2f startPosition(50, 50);
	setPosition(startPosition);

	// Set the initial state to StandingState
	m_state = std::make_unique<KirbyStandingState>();
	m_state->enter(*this);

	m_originalSpeed = m_speed; 
};

void Kirby::attack(std::vector<std::unique_ptr<Enemy>>& enemies, float range)
{
	if (dynamic_cast<KirbySparkAttackState*>(m_state.get()))
	{
		return;
	}
	// Check if Kirby has the Spark power and is in a state that allows attacking
	if (m_currentPower == PowerUpType::Spark &&
		(dynamic_cast<KirbyStandingState*>(m_state.get()) || dynamic_cast<KirbyWalkingState*>(m_state.get())))
	{
		// If so, transition to the special spark attack state
		m_state = std::make_unique<KirbySparkAttackState>(*this, enemies);
		m_state->enter(*this);
		return; // Exit to avoid the regular attack
	}
	
		bool facingLeft = m_velocity.x < 0 ? true : false;
		for (auto& enemy : enemies)
		{

			float distanceX = enemy->getPosition().x - getPosition().x;
			float distanceY = enemy->getPosition().y - getPosition().y;

			// The vertical tolerance is now much larger (ENTITY_SIZE / 2.0f),
			// allowing Kirby to swallow enemies that are not perfectly aligned.
			bool inVerticalRange = std::abs(distanceY) < (ENTITY_SIZE / 2.0f);
			bool inHorizontalRange = std::abs(distanceX) <= range;

			// An enemy is in range if it's within the vertical and horizontal distances.
			bool inRange = inVerticalRange && inHorizontalRange;

			if (collidesWith(*enemy))
				continue; // do nothing

			if (facingLeft && distanceX < 0 && inRange)
			{
				enemy->startBeingSwallowed();
				std::cout << "Swallowing enemy from the left!" << std::endl;
				break;
			}
			else if (distanceX > 0 && inRange)
			{
				enemy->startBeingSwallowed();
				std::cout << "Swallowing enemy from the right!" << std::endl;
				break;
			}
		}
	
}

// --- IMPLEMENT NEW POWER-UP FUNCTIONS ---
void Kirby::setPower(PowerUpType power)
{
	m_currentPower = power;
}

PowerUpType Kirby::getCurrentPower() const
{
	return m_currentPower;
}

void Kirby::update(float deltaTime)
{
	//m_isGrounded = false;

	// Manager handles all item effect timing and logic
	m_PowerUpManager.update(deltaTime, *this);
	
	if (m_isInvincible) // set to true in function takeDamage
		activateInvincibility(deltaTime);

	std::unique_ptr<KirbyState> newState = nullptr;

	if (isInWater())
	{
		//std::cout << "UPDATE_DEBUG: -----> Condition MET. Creating new SwimmingState." << std::endl;
		m_state = std::make_unique<KirbySwimmingState>();
	}
	else
	{
		//std::cout << "UPDATE_DEBUG: -----> handleInput returned a new state." << std::endl;
		newState = m_state->handleInput(*this);
	}

	if (newState)
	{
		//std::cout << "UPDATE_DEBUG: -----> Performing state transition..." << std::endl;
		m_state = std::move(newState);
		m_state->enter(*this);
	}
	m_state->update(*this, deltaTime);

	m_oldPosition = m_position;
	setPosition(m_position + m_velocity * deltaTime);
	GameObject::update(deltaTime);
}

//void Kirby::move(float deltaTime)
//{
//	m_oldPosition = m_position;
//	setPosition(m_position + m_velocity * deltaTime);
//}

void Kirby::handleCollision(GameObject* other)
{
	if (other->getType() == ObjectType::WALL)
	{
		sf::FloatRect wallBounds = other->getBounds();

		setVelocity(sf::Vector2f(0, 0));
		setPosition(getOldPosition());
	}
	else if (other->getType() == ObjectType::ENEMY)
	{
		static_cast<Enemy*>(other)->handleCollision(this);
	}
	else 
	{
		other->handleCollision(this);
	}
}

void Kirby::draw(sf::RenderTarget& target) const
{
	// First, draw the base Kirby sprite
	MovingObject::draw(target);

	// Then, allow the current state to draw any special effects over Kirby
	if (m_state)
	{
		m_state->draw(target);
	}
}

void Kirby::handleCollision(Door* door)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		door->handleCollision(this);
	}
}

// Decrease kirbys health by damageAmount
void Kirby::takeDamage(int damageAmount)
{
	// Ignore damage if invincible or no lives left
	if (m_isInvincible || m_lives <= 0) return;

	m_health -= damageAmount;

	if (m_health <= 0)
	{
		loseLife();
		m_health = m_maxHealth; // Reset health after losing a life
	}

	// Start invincibility period
	m_invincibilityTimer = 1.0f;
	m_isInvincible = true;

	std::cout << "Kirby took damage! Health: " << m_health << ", Lives: " << m_lives << std::endl;
}

void Kirby::heal(int healAmount)
{
	m_health = std::min(m_health + healAmount, m_maxHealth);
	std::cout << "Kirby healed! Health: " << m_health << std::endl;
}

void Kirby::loseLife()
{
	m_lives--;
	std::cout << "Life lost! Lives remaining: " << m_lives << std::endl;

	if (m_lives <= 0)
	{
		std::cout << "Game Over!" << std::endl;
	}
}

// Prevents rapid damage by setting invincibility timer
void Kirby::activateInvincibility(float deltaTime)
{
	m_invincibilityTimer -= deltaTime;
	if (m_invincibilityTimer <= 0.0f)
	{
		m_isInvincible = false;
	}

	// Make sprite flash during invincibility
	static float flashTimer = 0.0f;
	flashTimer += deltaTime;
	if (flashTimer > 0.1f) // Flash every 0.1 seconds
	{
		sf::Color currentColor = m_sprite.getColor();
		m_sprite.setColor(currentColor.a == 255 ? sf::Color(255, 255, 255, 128) : sf::Color::White);
		flashTimer = 0.0f;
	}
	if (!m_isInvincible)
	{
		m_sprite.setColor(sf::Color::White); // Ensure normal color once invinciblity is over
	}
}

float Kirby::getSpeed() const
{
	return m_speed;
}

void Kirby::setSpeed(float speed)
{
	m_speed = speed;
}

float Kirby::getOriginalSpeed() const
{
	return m_originalSpeed;
}

void Kirby::addPowerUpEffect(std::unique_ptr<PowerUpCommand> command)
{
	m_PowerUpManager.add(std::move(command), *this);
}

bool Kirby::isHyper() const { return m_isHyper; }
void Kirby::setHyper(bool hyper) { m_isHyper = hyper; }
bool Kirby::isInvincible() const { return m_isInvincible; }

// --- NEW PHYSICS METHOD DEFINITIONS ---

void Kirby::setVelocity(const sf::Vector2f& velocity) { m_velocity = velocity; }
sf::Vector2f Kirby::getVelocity() const { return m_velocity; }

void Kirby::setGrounded(bool grounded) { m_isGrounded = grounded; }
bool Kirby::isGrounded() const { return m_isGrounded; }

void Kirby::setInWater(bool inWater) { m_inWater = inWater; }
bool Kirby::isInWater() const { return m_inWater; }

// other getter/setters
int Kirby::getLives() const
{ 	
	return m_lives; 	
}
int Kirby::getHealth() const 
{ 
	return m_health; 
}
int Kirby::getMaxHealth() const 
{ 
	return m_maxHealth; 
}