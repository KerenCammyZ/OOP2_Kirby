#pragma once
#include <SFML/Graphics.hpp>
#include "GlobalSizes.h"

class GameObject
{
public:
	// constructor and destructor
	GameObject();
	virtual ~GameObject();

	// virtual methods
	virtual void update(float dt) = 0;
	virtual void onCollision(GameObject* other) {}

	void draw(sf::RenderTarget& target) const;

	// setters and getters
	void setPosition(const sf::Vector2f& position);
	sf::Vector2f getPosition() const;

	void setSize(const sf::Vector2f& size);;
	sf::Vector2f getSize() const;

	void setTexture(std::shared_ptr<sf::Texture> texture);
	sf::FloatRect getBounds() const;

protected:
	sf::Vector2f m_position;
	sf::Vector2f m_size;
	std::shared_ptr<sf::Texture> m_texture; // Use shared_ptr for texture management
	sf::Sprite m_sprite; // Sprite for rendering the texture
};