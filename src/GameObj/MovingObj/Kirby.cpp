#include "GameObj/MovingObj/Kirby.h"
#include "GameObj/MovingObj/Enemy.h"
#include "GameObj/FixedObj/Door.h"
#include "States/KirbyStandingState.h"
#include "GlobalSizes.h"
#include "GameObj/FixedObj/Wall.h"
#include <iostream>

// It's better to declare these constants where they are used,
// but for now, we'll make them accessible via GlobalSizes.h
// Make sure GlobalSizes.h is included in any file that needs them.

Kirby::Kirby(std::shared_ptr<sf::Texture>& kirbyTexture)
	: m_velocity(0.f, 0.f), m_isGrounded(false) // Initialize physics members
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
	m_direction = sf::Vector2f(1.f, 0.f); // Kirby starts facing right
};

// set facing direction
// @param direction -1 for left, 1 for right
void Kirby::faceDirection(int direction)
{
	m_direction.x = direction;
}

void Kirby::attack(std::vector<std::unique_ptr<Enemy>>& enemies, float range)
{
	bool facingLeft = m_velocity.x < 0 ? true : false;
	for (auto& enemy : enemies)
	{
		float distanceX = enemy->getPosition().x - getPosition().x;
		float distanceY = enemy->getPosition().y - getPosition().y;

		if (collidesWith(*enemy))
			; // do nothing
		else if (facingLeft && (distanceX < -range) && std::abs(distanceY) < 1.f) {
			enemy->onSwallowed();
			break;
		}
		else if (!facingLeft && distanceX < range && std::abs(distanceY) < 1.f) {
			enemy->onSwallowed();
			break;
		}
	}
}

void Kirby::update(float deltaTime)
{
	// Manager handles all item effect timing and logic
	m_presentManager.update(deltaTime, *this);

	// Handle invincibility frames 
	if (m_isInvincible)
	{
		// TODO: Consider moving this to a separate InvincibilityState
		// possible challenges:
		// 1. it should not cancel other states
		// 2. caused by collision, not by keyboard input
		m_invincibilityTimer -= deltaTime;
		if (m_invincibilityTimer <= 0.0f)
		{
			m_isInvincible = false;
		}

		// Optional: Make sprite flash during invincibility
		static float flashTimer = 0.0f;
		flashTimer += deltaTime;
		if (flashTimer > 0.1f) // Flash every 0.1 seconds
		{
			sf::Color currentColor = m_sprite.getColor();
			m_sprite.setColor(currentColor.a == 255 ? sf::Color(255, 255, 255, 128) : sf::Color::White);
			flashTimer = 0.0f;
		}
	}
	else
	{
		m_sprite.setColor(sf::Color::White); // Ensure normal color once invinciblity is over
	}

	// Assume we are not on the ground at the start of the frame.
	setGrounded(false);

	// Current state handle transitions and modify velocity.
	auto newState = m_state->handleInput(*this);
	if (newState)
	{
		m_state = std::move(newState);
		m_state->enter(*this);
	}
	m_state->update(*this, deltaTime);

	move(deltaTime); // Apply the final velocity to our position.
	GameObject::update(deltaTime);

	setGrounded(false); // Q: redundant? setGrounded is reset to false as required
						// in the next call to Kirby::Update() before modifying m_state 
}

void Kirby::move(float deltaTime)
{
	// If Kirby is not grounded, apply gravity
	m_oldPosition = m_position;
	setPosition(m_position + m_velocity * deltaTime);

	// If Kirby is not moving horizontally, keep the direction as it is.
}

void Kirby::handleCollision(GameObject* other)
{
	if (other->getType() == ObjectType::WALL)
	{
		setPosition(getOldPosition());
		setVelocity(sf::Vector2f(0, 0));
	}
	else if (other->getType() == ObjectType::ENEMY)
	{
		static_cast<Enemy*>(other)->handleCollision(this);
	}
	//else if (other->getType() == ObjectType::DOOR)
	//{
	//	handleCollision(static_cast<Door*>(other));
	//	return;
	//}
	else {
		other->handleCollision(this);
	}
}

void Kirby::handleCollision(Door* door)
{
	door->handleCollision(this);
}

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
	m_isInvincible = true;
	m_invincibilityTimer = 1.0f; // 1 second of invincibility

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

void Kirby::addPresentEffect(std::unique_ptr<PresentCommand> command)
{
	m_presentManager.add(std::move(command), *this);
}

// --- NEW PHYSICS METHOD DEFINITIONS ---
void Kirby::setVelocity(const sf::Vector2f& velocity) { m_velocity = velocity; }
sf::Vector2f Kirby::getVelocity() const { return m_velocity; }
void Kirby::setGrounded(bool grounded) { m_isGrounded = grounded; }
bool Kirby::isGrounded() const { return m_isGrounded; }