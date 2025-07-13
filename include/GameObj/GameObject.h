// GameObject.h
#pragma once
#include <SFML/Graphics.hpp>
#include "GlobalSizes.h"

enum class ObjectType 
  { DOOR, 
	FLOOR, 
	WALL, 
	ENEMY, 
	POWERUP,  
	EXIT ,
	WATER, 
	DEFAULT,
	SPIKE,};

class Kirby;
class Door;
class Enemy;
class Spike;

class GameObject
{
public:
	GameObject();
	GameObject(const std::shared_ptr<sf::Texture>& texture);
	virtual ~GameObject() = default;

	virtual void update(float dt);
	virtual void draw(sf::RenderTarget& target) const;

	virtual void handleCollision(GameObject* other) = 0;
	virtual void handleCollision(Kirby* kirby) = 0;
	virtual void handleCollision(Door* door) = 0;
	virtual void handleCollision(Enemy* enemy) = 0;
	virtual void handleCollision(Spike* spike) = 0;

	// --- VIRTUAL SETTERS AND GETTERS ---
	virtual void setPosition(const sf::Vector2f& position);
	sf::Vector2f getPosition() const;

	virtual void setSize(const sf::Vector2f& size);
	sf::Vector2f getSize() const;

	void setTexture(std::shared_ptr<sf::Texture> texture);
	bool collidesWith(GameObject& other) const;

	virtual sf::FloatRect getBounds() const;
	virtual ObjectType getType() const { return ObjectType::DEFAULT; }

protected:
	sf::Vector2f m_position;
	sf::Vector2f m_size;
	sf::Sprite m_sprite;
	std::shared_ptr<sf::Texture> m_texture;
};
