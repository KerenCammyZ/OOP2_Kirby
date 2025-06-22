#include "GameObj/MovingObj/Kirby.h"
#include "GameObj/FixedObj/Door.h"
#include "States/KirbyStandingState.h"
#include "GlobalSizes.h"

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
};

// This is the main update function and the ONLY one that should contain this logic.
void Kirby::update(float deltaTime)
{
	// 1. Assume we are not on the ground at the start of the frame.
	//    The collision check later will correct this if we are.
	//setGrounded(false);

	// 2. Let the current state handle transitions and modify velocity.
	auto newState = m_state->handleInput(*this);
	if (newState)
	{
		m_state = std::move(newState);
		m_state->enter(*this);
	}
	m_state->update(*this, deltaTime);

	// 3. Apply the final velocity to our position.
	//    This is where all movement actually happens.
	m_oldPosition = m_position;
	setPosition(m_position + m_velocity * deltaTime);
	GameObject::update(deltaTime);

	setGrounded(false);
}


void Kirby::handleCollision(GameObject* other)
{
	other->handleCollision(this);
}

void Kirby::handleCollision(Door* door)
{
	door->handleCollision(this);
}

float Kirby::getSpeed() const
{
	return m_speed;
}

// --- NEW PHYSICS METHOD DEFINITIONS ---
void Kirby::setVelocity(const sf::Vector2f& velocity) { m_velocity = velocity; }
sf::Vector2f Kirby::getVelocity() const { return m_velocity; }
void Kirby::setGrounded(bool grounded) { m_isGrounded = grounded; }
bool Kirby::isGrounded() const { return m_isGrounded; }