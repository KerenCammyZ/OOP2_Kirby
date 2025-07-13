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
	: m_velocity(0.f, 0.f), m_isGrounded(false), m_inWater(false), // Initialize physics members
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
};

void Kirby::setupAnimations() {
	m_animator->addGridAnimation("idle", 0, 0, 16, 16, 1, 0.5f, true);
	m_animator->addGridAnimation("walking", 0, 16, 16, 16, 4, 0.15f, false);
	m_animator->addGridAnimation("falling", 0, 31, 16, 16, 1, 0.3f, true);
	m_animator->addGridAnimation("swallowing", 0, 48, 16, 16, 3, 0.2f, false);
	m_animator->addGridAnimation("jumping", 0, 63, 16, 16, 2, 0.2f, false);
	m_animator->addGridAnimation("swimming", 0, 79, 16, 16, 4, 0.25f, true);
	m_animator->addGridAnimation("spark_attack", 0, 175, 16, 16, 4, 0.5f, false);
	//m_animator->addGridAnimation("water_attack", 16, 111, 16, 16, 3, 0.5f, false);
}

void Kirby::setAnimation(const std::string& name) {
	if (m_animator)
		m_animator->play(name);
}

void Kirby::updateAnimation(float deltaTime) {
	if (!m_animator) return;

	m_animationChangeDelay -= deltaTime;
	
	// Update facing direction based on actual movement
	if (std::abs(m_velocity.x) > 10.0f) {  // Only change direction when actually moving
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
	// If Kirby is in the swimming state, perform the water fountain attack.
	if (dynamic_cast<KirbySwimmingState*>(m_state.get()))
	{
		this->setState(std::make_unique<KirbyWaterAttackState>(*this, enemies));
		return; // Exit to prevent other attacks from running.
	}

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
	
	bool facingLeft = (m_facingDirection == FacingDirection::Left);
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
			setAnimation("swallowing");
			std::cout << "Swallowing enemy from the left!" << std::endl;
			break;
		}
		else if (distanceX > 0 && inRange)
		{
			enemy->startBeingSwallowed();
			setAnimation("swallowing");
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

	// Then, allow the current state to draw any special effects over Kirby
	if (m_state)
	{
		m_state->draw(target);
	}
}

void Kirby::update(float deltaTime)
{
	// Manager handles all item effect timing and logic
	m_PowerUpManager.update(deltaTime, *this);

	if (m_animator) m_animator->update(deltaTime);
	
	if (m_isInvincible) // set to true in function takeDamage
		activateInvincibility(deltaTime);

	std::unique_ptr<KirbyState> newState = nullptr;

	// High-priority check: Should we enter the water?
	// This only runs if we are in the water but not already in a water-based state.
	if (isInWater() &&
		!dynamic_cast<KirbySwimmingState*>(m_state.get()) &&
		!dynamic_cast<KirbyWaterAttackState*>(m_state.get()))
	{
		newState = std::make_unique<KirbySwimmingState>();
	}
	else
	{
		// If no high-priority change, let the current state handle input.
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
			m_velocity.x = 0.f; // Stop horizontal movement if blocked
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
			m_velocity.y = 0.f; // Stop vertical movement if blocked
		}
	}

	m_oldPosition = m_position;
	setPosition(m_position + m_velocity * deltaTime);

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
	else if (other->getType() == ObjectType::FLOOR)
	{
		sf::FloatRect kirbyBounds = getBounds();
		sf::FloatRect floorBounds = other->getBounds();
		sf::Vector2f kirbyPrevPos = getOldPosition();
		const int floorHeight = floorBounds.height;

		// Check if Kirby was coming from above in the previous frame
		if (kirbyPrevPos.y + kirbyBounds.height / 2.f <= floorBounds.top)
		{
			// 1. Stop his vertical movement completely.
			setVelocity({ getVelocity().x, 0.f });

			// 2. Mark Kirby as being on the ground.
			setGrounded(true);

			// 3. Reposition him to be exactly on top of the floor.
			setPosition({ getPosition().x, floorBounds.top - (kirbyBounds.height / 2.f) });
		}

		else if (kirbyPrevPos.x + kirbyBounds.width / 2.f <= floorBounds.left) // Coming from the left
		{
			// Stop horizontal movement and reposition Kirby to the left and on top of the floor
			setVelocity({ 0.f, getVelocity().y });
			setPosition({ floorBounds.left - (kirbyBounds.width / 2.f), getPosition().y - floorHeight });
		}
		else if (kirbyPrevPos.x - kirbyBounds.width / 2.f >= floorBounds.left + floorBounds.width) // Coming from the right
		{
			// Stop horizontal movement and reposition Kirby to the right and on top of the floor
			setVelocity({ 0.f, getVelocity().y });
			setPosition({ floorBounds.left + floorBounds.width + (kirbyBounds.width / 2.f), getPosition().y - floorHeight });
		}
	}
	else 
	{
		other->handleCollision(this);
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


void Kirby::addLife(int lifeAmount)
{
	m_lives += lifeAmount;
	//std::cout << "Life added! Lives remaining: " << m_lives << std::endl;
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

// This is the double-dispatch target. It just calls the specific handler.
void Kirby::handleCollision(Spike* spike)
{
	// First, check if Kirby is already invincible from another source.
	if (isInvincible()) return;


	// If we are not immune, take 1 damage.
	takeDamage(1);
}