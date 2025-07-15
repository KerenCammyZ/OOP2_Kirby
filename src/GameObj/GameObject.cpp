// GameObject.cpp
#include "GameObj/GameObject.h"
#include <iostream> 

GameObject::GameObject()
{
	setPosition(sf::Vector2f(0.f, 0.f)); 
	setSize(sf::Vector2f(ENTITY_SIZE, ENTITY_SIZE)); 
}

GameObject::GameObject(const std::shared_ptr<sf::Texture>& texture)
{
	setTexture(texture);
	setPosition(sf::Vector2f(0.f, 0.f));
	setSize(sf::Vector2f(ENTITY_SIZE, ENTITY_SIZE)); 
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

// Updates sprite position and origin based on the current position and texture size
void GameObject::update(float dt)
{
	if (m_texture) {
		sf::Vector2u texSize = m_texture->getSize();  
		m_sprite.setOrigin(texSize.x / 2.f, texSize.y / 2.f);
	}
	m_sprite.setPosition(m_position);
}

void GameObject::setPosition(const sf::Vector2f& position)
{
	m_position = position;
	m_sprite.setOrigin(m_size.x / 2.f, m_size.y / 2.f);
	m_sprite.setPosition(m_position);
}

sf::Vector2f GameObject::getPosition() const
{
	return m_position;
}

// Sets GameObject size and centers origin by scaling sprite to match texture dimensions
void GameObject::setSize(const sf::Vector2f& size)
{
	m_size = size;

	if (m_texture)
	{
		sf::Vector2u textureSize = m_texture->getSize(); // original size in pixels

		// If texture size is valid, scale the sprite to match the desired size
		if (textureSize.x > 0 && textureSize.y > 0)
		{			
			float scaleX = size.x / textureSize.x;
			float scaleY = size.y / textureSize.y;
			m_sprite.setScale(scaleX, scaleY);
		}
	}
	m_sprite.setOrigin(m_size.x / 2.f, m_size.y / 2.f);
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
	if (m_texture) {
		sf::Vector2u texSize = m_texture->getSize();
		m_sprite.setOrigin(texSize.x / 2.f, texSize.y / 2.f);
	}
}

sf::FloatRect GameObject::getBounds() const
{
	return m_sprite.getGlobalBounds();
}