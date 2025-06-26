// Kirby.h
#pragma once
#include "GameObj/MovingObj/MovingObject.h"
#include "PresentManager.h" 
#include <memory>

class KirbyState;
class Wall;
class PresentCommand;

class Kirby : public MovingObject
{
public:
	Kirby(std::shared_ptr<sf::Texture>& kirbyTexture);

	// We override MovingObject's update to handle our new physics logic
	void update(float deltaTime) override;
	void move(float deltaTime) override {};

	float getSpeed() const;
	void setSpeed(float speed);
	float getOriginalSpeed() const;

	// The single public method to add any present's effect
	void addPresentEffect(std::unique_ptr<PresentCommand> command);

	// --- NEW PHYSICS GETTERS/SETTERS ---
	void setVelocity(const sf::Vector2f& velocity);
	sf::Vector2f getVelocity() const;

	void setGrounded(bool grounded);
	bool isGrounded() const;

	// Collision Handlers
	void handleCollision(GameObject* other) override;
	void handleCollision(Kirby* kirby) override {};
	void handleCollision(Door* door) override;

	void handleCollision(Wall* wall);

private:
	std::unique_ptr<KirbyState> m_state;

	PresentManager m_presentManager;
	float m_originalSpeed;

	// --- NEW PHYSICS MEMBERS ---
	sf::Vector2f m_velocity;
	bool m_isGrounded;
};