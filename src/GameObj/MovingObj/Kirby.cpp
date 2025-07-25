#include "States/KirbyStates/KirbyStandingState.h"
#include "States/KirbyStates/KirbySwimmingState.h"
#include "States/KirbyStates/KirbySparkAttackState.h"
#include "States/KirbyStates/KirbyWaterAttackState.h" 
#include "States/KirbyStates/KirbyWalkingState.h" 
#include "States/KirbyStates/KirbyJumpingState.h" 
#include "States/KirbyStates/KirbyFallingState.h" 
#include "GlobalSizes.h"
#include "GameObj/FixedObj/Wall.h"
#include "GameObj/FixedObj/Door.h"
#include "GameObj/MovingObj/Kirby.h"
#include "GameObj/MovingObj/Enemy.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <typeinfo> 

Kirby::Kirby(const std::shared_ptr<sf::Texture>& kirbyTexture)
	: m_velocity(0.f, 0.f), m_isGrounded(false), m_inWater(false),
	m_lastAnimationState("idle"), m_animationChangeDelay(0.0f)
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

	m_animator = std::make_unique<Animator>();
	if (m_animator->loadSpriteSheet("KirbySpriteSheet.png")) {
		setupAnimations();
		m_animator->play("idle");
	}
	// --- ADD THIS INITIALIZATION LOGIC ---
	// Configure the ground sensor rectangle
	float sensorWidth = getSize().x * 0.8f; // Slightly narrower than Kirby
	float sensorHeight = 5.f; // A few pixels high
	m_groundSensor.setSize({ sensorWidth, sensorHeight });
	m_groundSensor.setFillColor(sf::Color(0, 255, 0, 150)); // Green and semi-transparent for debugging
	m_groundSensor.setOrigin(sensorWidth / 2.f, sensorHeight / 2.f);
}

void Kirby::setupAnimations() {
	m_animator->addGridAnimation("idle", 0, 0, 16, 16, 1, 0.5f, true);
	m_animator->addGridAnimation("walking", 0, 16, 16, 16, 4, 0.15f, true);
	m_animator->addGridAnimation("falling", 0, 31, 16, 16, 1, 0.3f, true);
	m_animator->addGridAnimation("swallowing", 0, 48, 16, 16, 3, 0.2f, false);
	m_animator->addGridAnimation("jumping", 0, 63, 16, 16, 2, 0.2f, false);
	m_animator->addGridAnimation("swimming", 0, 79, 16, 16, 4, 0.25f, true);
	m_animator->addGridAnimation("spark_attack", 0, 175, 16, 16, 4, 0.5f, false);
}

void Kirby::setAnimation(const std::string& name) {
	if (m_animator)
		m_animator->play(name);
}

void Kirby::updateAnimation(float deltaTime) {
	if (!m_animator) return;

	m_animationChangeDelay -= deltaTime;
	
	// Update facing direction based on actual movement
	if (std::abs(m_velocity.x) > 10.0f) {
		m_facingLeft = (m_velocity.x < 0);
	}

	std::string targetAnimation = "idle";

	if (isInWater()) {
		targetAnimation = "swimming";
	}
	else if (std::abs(m_velocity.x) > 20.0f){
		targetAnimation = "walking";
	}
	else if (m_velocity.y < -100.0f) {
		targetAnimation = "jumping";
	}
	else if (m_velocity.y > 100.0f) {
		targetAnimation = "falling";
	}

	if (targetAnimation != m_lastAnimationState && m_animationChangeDelay <= 0.0f) {
		m_animator->play(targetAnimation);
		m_lastAnimationState = targetAnimation;
		m_animationChangeDelay = 0.1f;
	}

	m_animator->update(deltaTime);
}

void Kirby::attack(std::vector<std::unique_ptr<Enemy>>& enemies, float range)
{
	if (dynamic_cast<KirbySwimmingState*>(m_state.get()))
	{
		this->setState(std::make_unique<KirbyWaterAttackState>(*this, enemies));
		return; // Exit to prevent other attacks from running.
	}

	if (dynamic_cast<KirbySparkAttackState*>(m_state.get()))
	{
		return;
	}
	// Check if Kirby has the Spark power
	if (m_currentPower == PowerUpType::Spark)
	{
		m_state = std::make_unique<KirbySparkAttackState>(*this, enemies);
		m_state->enter(*this);
		return; // Exit to avoid the regular attack
	}
	
	bool facingLeft = (m_facingDirection == FacingDirection::Left);
	for (auto& enemy : enemies)
	{

		float distanceX = enemy->getPosition().x - getPosition().x;
		float distanceY = enemy->getPosition().y - getPosition().y;

		bool inVerticalRange = std::abs(distanceY) < (ENTITY_SIZE / 2.0f);
		bool inHorizontalRange = std::abs(distanceX) <= range;

		// An enemy is in range if it's within the vertical and horizontal distances.
		bool inRange = inVerticalRange && inHorizontalRange;

		if (collidesWith(*enemy))
			continue; // do nothing being swallowed

		if ((facingLeft && distanceX < 0 && inRange) || (distanceX > 0 && inRange))
		{
			enemy->startBeingSwallowed();
			setAnimation("swallowing");
			break;
		}
	}
}

void Kirby::setPower(PowerUpType power)
{
	m_currentPower = power;
}

PowerUpType Kirby::getCurrentPower() const
{
	return m_currentPower;
}

void Kirby::draw(sf::RenderTarget& target) const
{
	float scaleX = isFacingLeft() ? -1.0f : 1.0f;
	float scaleY = 1.0f;
	
	float entityScale = ENTITY_SIZE / 16.0f;
	scaleX *= entityScale;
	scaleY *= entityScale;

	if (m_animator)
		m_animator->draw(target, getPosition().x, getPosition().y, scaleX, scaleY);
	else
		MovingObject::draw(target);

	// allow the current state to draw any special effects over Kirby
	if (m_state)
	{
		m_state->draw(target);
	}

	//target.draw(m_groundSensor);
}

void Kirby::update(float deltaTime)
{
	// Manager handles all item effect timing and logic
	m_PowerUpManager.update(deltaTime, *this);

	if (m_animator) m_animator->update(deltaTime);
	
	if (m_isInvincible)
		activateInvincibility(deltaTime);

	std::unique_ptr<KirbyState> newState = nullptr;

	bool isAirborne = dynamic_cast<KirbyAirborneState*>(m_state.get()) ||
		dynamic_cast<KirbyJumpingState*>(m_state.get());


	if (!isGrounded() && !isInWater() && !isAirborne)
	{
		// If not grounded, not in water, and not already in an air-state, we MUST be falling.
		newState = std::make_unique<KirbyFallingState>();
	}

	// Should we enter the water
	if (isInWater() &&
		!dynamic_cast<KirbySwimmingState*>(m_state.get()) &&
		!dynamic_cast<KirbyWaterAttackState*>(m_state.get()))
	{
		newState = std::make_unique<KirbySwimmingState>();
	}
	else
	{
		newState = m_state->handleInput(*this);
	}

	if (newState) {
		m_state = std::move(newState);
		m_state->enter(*this);
	}
	m_state->update(*this, deltaTime);
	updateAnimation(deltaTime);
}

void Kirby::move(float deltaTime, const std::vector<std::unique_ptr<GameObject>>& obstacles)
{
	sf::Vector2f newPosition = m_position;

	// Move along X axis
	float dx = m_velocity.x * deltaTime;
	if (dx != 0.f) {
		sf::Vector2f testPosX = { newPosition.x + dx, newPosition.y };
		if (!willCollideAt(testPosX, obstacles)) {
			newPosition.x += dx;
		}
		else {
			m_velocity.x = 0.f;
		}
	}


	// Move along Y axis
	float dy = m_velocity.y * deltaTime;
	if (dy != 0.f) {
		sf::Vector2f testPosY = { newPosition.x, newPosition.y + dy };
		if (!willCollideAt(testPosY, obstacles)) {
			newPosition.y += dy;
		}
		else {
			m_velocity.y = 0.f;
		}
	}

	m_oldPosition = m_position;
	setPosition(m_position + m_velocity * deltaTime);
	updateGroundSensor();

	float currentScaleX = m_sprite.getScale().x;
	m_sprite.setScale(std::abs(currentScaleX) * static_cast<float>(m_facingDirection), m_sprite.getScale().y);

	GameObject::update(deltaTime);
}

void Kirby::setFacingDirection(FacingDirection dir)
{
	m_facingDirection = dir;
}

FacingDirection Kirby::getFacingDirection() const
{
	return m_facingDirection;
}

void Kirby::setState(std::unique_ptr<KirbyState> state)
{
	if (state)
	{
		m_state = std::move(state);
		m_state->enter(*this);
	}
	else
	{
		std::cerr << "Error: Attempted to set Kirby state to nullptr!" << std::endl;
	}
}

bool Kirby::willCollideAt(const sf::Vector2f& intendedPosition, const std::vector<std::unique_ptr<GameObject>>& obstacles) const
{
	// Create a temporary bounding box for the intended position
	sf::FloatRect intendedBounds = getBounds();
	intendedBounds.left = intendedPosition.x - intendedBounds.width / 2.f;
	intendedBounds.top = intendedPosition.y - intendedBounds.height / 2.f;

	for (const auto& obj : obstacles) {
		if (obj->getType() == ObjectType::WALL || obj->getType() == ObjectType::FLOOR) {
			if (intendedBounds.intersects(obj->getBounds())) {
				return true;
			}
		}
	}
	return false;
}

void Kirby::handleCollision(GameObject* other)
{
	other->handleCollision(this);
}

void Kirby::updateGroundSensor()
{
	float yOffset = getSize().y / 2.f;
	m_groundSensor.setPosition(getPosition().x, getPosition().y + yOffset);
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
	if (m_isInvincible || m_lives < 0) return;

	m_health -= damageAmount;

	if (m_health <= 0)
	{
		loseLife();
		//m_health = m_maxHealth; // Reset health after losing a life
		if (m_lives > 0)
			m_health = m_maxHealth; // Only reset health if lives remain
		else
			m_health = 0; // Ensure health is 0 if no lives left
	}

	// Start invincibility period
	m_invincibilityTimer = 1.0f;
	m_isInvincible = true;

}

void Kirby::heal(int healAmount)
{
	m_health = std::min(m_health + healAmount, m_maxHealth);
}

void Kirby::loseLife()
{
	m_lives--;
}


void Kirby::addLife(int lifeAmount)
{
	m_lives += lifeAmount;
	if (m_lives > 9) // Cap lives at 9
	{
		m_lives = 9;
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
		// Ensure normal color once invinciblity is over
		m_sprite.setColor(sf::Color::White); 
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

void Kirby::setVelocity(const sf::Vector2f& velocity) { m_velocity = velocity; }
sf::Vector2f Kirby::getVelocity() const { return m_velocity; }

void Kirby::setGrounded(bool grounded) { m_isGrounded = grounded; }
bool Kirby::isGrounded() const { return m_isGrounded; }

void Kirby::setInWater(bool inWater) { m_inWater = inWater; }
bool Kirby::isInWater() const { return m_inWater; }

int Kirby::getLives() const { return m_lives; }
int Kirby::getHealth() const { return m_health; }
int Kirby::getMaxHealth() const { return m_maxHealth; }

void Kirby::setLife(int life)
{
	m_lives = life;
}

void Kirby::setHealth(int health)
{
	m_health = health;
}


void Kirby::handleCollision(Spike* spike)
{
	if (isInvincible())
		return;

	takeDamage(1);
}