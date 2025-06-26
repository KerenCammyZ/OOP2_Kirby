#include "GameObj/GameObject.h"
#include <iostream> 

GameObject::GameObject()
{
	m_position = sf::Vector2f(0.f, 0.f); // Initialize position to (0, 0)
	m_size = sf::Vector2f(ENTITY_SIZE, ENTITY_SIZE); // Default size based on ENTITY_SIZE
	m_texture = nullptr; // No texture by default
	m_sprite.setOrigin(m_size.x / 2.f, m_size.y / 2.f); // Center the sprite origin
	m_sprite.setPosition(m_position); // Set initial position of the sprite
}

bool GameObject::collidesWith(GameObject& other) const
{
	return getBounds().intersects(other.getBounds());
}

void GameObject::draw(sf::RenderTarget& target) const
{
	if (m_texture)
	{
		target.draw(m_sprite);
	}
}


void GameObject::update(float dt)
{
	if (m_texture) {
		sf::Vector2u texSize = m_texture->getSize();      // 16×16
		m_sprite.setOrigin(texSize.x / 2.f, texSize.y / 2.f);  // Origin at (8, 8)
	}
	m_sprite.setPosition(m_position); // NOTICE: relevant to moving objects only
}

void GameObject::setPosition(const sf::Vector2f& position)
{
	m_position = position;
}

sf::Vector2f GameObject::getPosition() const
{
	return m_position;
}

// sets the size of the GameObject:
// updates m_size and sets m_sprite scale accordingly
void GameObject::setSize(const sf::Vector2f& size)
{
	m_size = size;

	if (m_texture)	// Check if the texture is loaded before trying to get its size
	{
		sf::Vector2u textureSize = m_texture->getSize(); // original size in pixels

		if (textureSize.x > 0 && textureSize.y > 0) // Avoid division by zero if texture is empty
		{
			// Calculate the scale factors by dividing the desired size by the texture's original size
			// (determines how much the texture should be scaled to fit the desired size)
			float scaleX = size.x / textureSize.x;
			float scaleY = size.y / textureSize.y;
			// Set the sprite's scale to match the desired size
			m_sprite.setScale(scaleX, scaleY);
		}
	}
}

sf::Vector2f GameObject::getSize() const
{
	return m_size;
}

// Set the texture for the GameObject and update the sprite accordingly
void GameObject::setTexture(std::shared_ptr<sf::Texture> texture)
{
	m_texture = texture;
	m_sprite.setTexture(*m_texture);
}

sf::FloatRect GameObject::getBounds() const
{
	// The origin is now centered, so adjust the position for the bounding box
	sf::Vector2f cornerPosition = m_position - sf::Vector2f(m_size.x / 2.f, m_size.y / 2.f);
	return sf::FloatRect(cornerPosition, m_size);
}