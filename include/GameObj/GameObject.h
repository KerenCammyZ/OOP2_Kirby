// GameObject.h
#pragma once
#include <SFML/Graphics.hpp>
#include "GlobalSizes.h"

enum class ObjectType { WALL, GIFT, ENEMY, SPEED_PRESENT, DEFAULT };

class Kirby;
class Door;

class GameObject
{
public:
	GameObject();
	virtual ~GameObject() = default;

	virtual void update(float dt);
	virtual void draw(sf::RenderTarget& target) const;

	virtual void handleCollision(GameObject* other) = 0;
	virtual void handleCollision(Kirby* kirby) = 0;
	virtual void handleCollision(Door* door) = 0;

	// enforce override in moving objects only
	// virtual void handleCollision(Wall* wall) {}

	// --- VIRTUAL SETTERS AND GETTERS ---
	virtual void setPosition(const sf::Vector2f& position);
	sf::Vector2f getPosition() const;

	virtual void setSize(const sf::Vector2f& size);
	sf::Vector2f getSize() const;

	bool collidesWith(GameObject& other) const;
	virtual sf::FloatRect getBounds() const;
	void setTexture(std::shared_ptr<sf::Texture> texture);

	virtual ObjectType getType() const { return ObjectType::DEFAULT; }

protected:
	sf::Vector2f m_position;
	sf::Vector2f m_size;
	std::shared_ptr<sf::Texture> m_texture;
	sf::Sprite m_sprite;
};
