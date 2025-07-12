#include "GameObj/MovingObj/Kirby.h"
#include "GameObj/MovingObj/Enemy.h"
#include "GameObj/FixedObj/Door.h"
#include "States/KirbyStates/KirbyStandingState.h"
#include "States/KirbyStates/KirbySwimmingState.h"
#include "GlobalSizes.h"
#include "GameObj/FixedObj/Wall.h"
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

	// Create animation manager first (without spritesheet)
	m_animationManager = std::make_unique<AnimationManager>();
	m_currentAnimationState = "idle";

	// Setup animation system
	m_spriteSheet = std::make_shared<SpriteSheet>();
	if (m_spriteSheet->loadTexture("KirbySpriteSheet.png")) {
		// Initialize the animation manager with the loaded spritesheet
		m_animationManager->initialize(m_spriteSheet);
		setupAnimations();
	}
	else {
		std::cout << "Warning: Could not load KirbySpriteSheet.png - animations disabled" << std::endl;
	}
};

void Kirby::debugAnimationState() const {
	if (m_animationManager && m_animationManager->isInitialized()) {
		std::cout << "Current animation: " << m_currentAnimationState
			<< " | Sprite: " << m_animationManager->getCurrentSpriteName() << std::endl;

		Animation* currentAnim = m_animationManager->getCurrentAnimation();
		if (currentAnim) {
			std::cout << "Animation playing: " << currentAnim->isPlaying()
				<< " | Finished: " << currentAnim->isFinished()
				<< " | Frame count: " << currentAnim->getFrameCount() << std::endl;
		}
	}
	else {
		std::cout << "Animation manager not initialized!" << std::endl;
	}
}

std::string Kirby::getCurrentAnimationName() const {
	if (m_animationManager) {
		return m_animationManager->getCurrentAnimationName();
	}
	return "No animation manager";
}

void Kirby::attack(std::vector<std::unique_ptr<Enemy>>& enemies, float range)
{
	//bool facingLeft = m_velocity.x < 0 ? true : false;
	for (auto& enemy : enemies)
	{
		float distanceX = enemy->getPosition().x - getPosition().x;
		float distanceY = enemy->getPosition().y - getPosition().y;
		bool inRange = std::abs(distanceX) <= range && std::abs(distanceY) < 2.5f;

		if (collidesWith(*enemy))
			continue; // do nothing

		if (inRange && isFacingLeft() && distanceX < 0)
		{
			enemy->onSwallowed();
			std::cout << "Swallowed enemy from the left!" << std::endl;
			break;
		}
		else if (inRange && distanceX > 0)
		{
			enemy->onSwallowed();
			std::cout << "Swallowed enemy from the right!" << std::endl;
			break;
		}
	}
}

// Modified update method with debug output:
void Kirby::update(float deltaTime)
{
	// Your existing update logic...
	m_presentManager.update(deltaTime, *this);

	if (m_isInvincible) {
		activateInvincibility(deltaTime);
	}

	std::unique_ptr<KirbyState> newState = nullptr;

	if (isInWater()) {
		m_state = std::make_unique<KirbySwimmingState>();
	}
	else {
		newState = m_state->handleInput(*this);
	}

	if (newState) {
		m_state = std::move(newState);
		m_state->enter(*this);
	}
	m_state->update(*this, deltaTime);

	// Update animations based on current state
	std::string currentStateName = typeid(*m_state).name();
	updateAnimation(currentStateName, deltaTime);

	// Debug output (remove this later)
	static float debugTimer = 0;
	debugTimer += deltaTime;
	if (debugTimer > 1.0f) { // Print debug info every second
		debugAnimationState();
		debugTimer = 0;
	}
}

// Enhanced updateAnimation with more debug info:
void Kirby::updateAnimation(const std::string& stateName, float deltaTime) {
	if (!m_animationManager || !m_animationManager->isInitialized()) {
		return;
	}

	std::string newAnimationState;

	// Determine animation based on physics state rather than class names
	if (isInWater()) {
		newAnimationState = "swimming";
	}
	else {
		bool isMoving = std::abs(m_velocity.x) > 10.0f; // Small threshold to avoid jitter

		// Check if we're airborne (you might need to add this check)
		bool isAirborne = !isGrounded();

		if (isAirborne) {
			if (m_velocity.y < -20.0f) {
				newAnimationState = "jumping";
			}
			else {
				newAnimationState = "falling";
			}
		}
		else if (isMoving) {
			newAnimationState = "walking";
		}
		else {
			newAnimationState = "idle";
		}
	}

	// Only change if different AND use smooth transition
	if (newAnimationState != m_currentAnimationState) {
		m_currentAnimationState = newAnimationState;

		// Use smooth animation change (add this method to AnimationManager)
		if (m_animationManager->hasAnimation(newAnimationState)) {
			m_animationManager->playAnimationSmooth(newAnimationState);
		}
	}

	m_animationManager->update(deltaTime);
}

// Enhanced setupAnimations with better debugging:
void Kirby::setupAnimations() {
	if (!m_animationManager || !m_animationManager->isInitialized()) {
		std::cerr << "AnimationManager not properly initialized!" << std::endl;
		return;
	}

	try {
		std::cout << "Setting up Kirby animations..." << std::endl;

		// Define sprite locations in your Kirby spritesheet
		m_spriteSheet->addSpriteGrid("kirby_idle", 0, 0, 16, 16, 1, 1);
		std::cout << "Added idle sprites" << std::endl;

		m_spriteSheet->addSpriteGrid("kirby_walk", 0, 16, 16, 16, 4, 4);
		std::cout << "Added walking sprites" << std::endl;

		m_spriteSheet->addSpriteGrid("kirby_fall", 0, 32, 16, 16, 1, 1);
		std::cout << "Added falling sprites" << std::endl;

		m_spriteSheet->addSpriteGrid("kirby_jump", 0, 64, 16, 16, 2, 2);
		std::cout << "Added jumping sprites" << std::endl;

		m_spriteSheet->addSpriteGrid("kirby_swim", 0, 128, 32, 32, 4, 4);
		std::cout << "Added swimming sprites" << std::endl;

		// Create animations using the AnimationManager
		Animation* idleAnim = m_animationManager->createAnimation("idle", true);
		if (idleAnim) {
			idleAnim->addFrameSequence("kirby_idle", 0, 0, 0.4f);
			std::cout << "Created idle animation with " << idleAnim->getFrameCount() << " frames" << std::endl;
		}

		Animation* walkAnim = m_animationManager->createAnimation("walking", true);
		if (walkAnim) {
			walkAnim->addFrameSequence("kirby_walk", 0, 3, 0.3f);
			std::cout << "Created walking animation with " << walkAnim->getFrameCount() << " frames" << std::endl;
		}

		Animation* jumpAnim = m_animationManager->createAnimation("jumping", false);
		if (jumpAnim) {
			jumpAnim->addFrameSequence("kirby_jump", 0, 1, 0.15f);
			std::cout << "Created jumping animation with " << jumpAnim->getFrameCount() << " frames" << std::endl;
		}

		Animation* fallAnim = m_animationManager->createAnimation("falling", true);
		if (fallAnim) {
			fallAnim->addFrameSequence("kirby_fall", 0, 0, 0.3f);
			std::cout << "Created falling animation with " << fallAnim->getFrameCount() << " frames" << std::endl;
		}

		Animation* swimAnim = m_animationManager->createAnimation("swimming", true);
		if (swimAnim) {
			swimAnim->addFrameSequence("kirby_swim", 0, 3, 0.2f);
			std::cout << "Created swimming animation with " << swimAnim->getFrameCount() << " frames" << std::endl;
		}

		// Start with idle animation
		if (m_animationManager->playAnimation("idle")) {
			std::cout << "Started idle animation successfully" << std::endl;
		}
		else {
			std::cout << "Failed to start idle animation" << std::endl;
		}

		std::cout << "Kirby animations setup completed!" << std::endl;

	}
	catch (const std::exception& e) {
		std::cerr << "Error setting up Kirby animations: " << e.what() << std::endl;
	}
}

// Modified draw method to handle size scaling:
void Kirby::draw(sf::RenderTarget& target) const {
	if (m_animationManager && m_animationManager->isInitialized()) {
		sf::Vector2f pos = getPosition();

		// Handle sprite flipping based on movement direction
		float scaleX = isFacingLeft() ? -1.0f : 1.0f;
		float scaleY = 1.0f;

		// Scale up the 16x16 sprites to match your entity size
		float entityScale = ENTITY_SIZE / 16.0f; // Assuming ENTITY_SIZE is your desired size
		scaleX *= entityScale;
		scaleY *= entityScale;

		// Adjust position when flipping to prevent sprite shifting
		if (isFacingLeft()) {
			pos.x += getSize().x / 2.0f;
		}

		m_animationManager->draw(target, pos.x, pos.y, scaleX, scaleY);
	}
	else {
		// Fallback to original draw method
		GameObject::draw(target);
	}
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
	setPosition(newPosition);
	GameObject::update(deltaTime);
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

void Kirby::addPresentEffect(std::unique_ptr<PresentCommand> command)
{
	m_presentManager.add(std::move(command), *this);
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