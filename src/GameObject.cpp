#include "GameObject.h"

GameObject::GameObject(){}

GameObject::~GameObject() {}

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
	m_sprite.setPosition(m_position);
}

void GameObject::setPosition(const sf::Vector2f& position)
{
	m_position = position;
}

sf::Vector2f GameObject::getPosition() const
{
	return m_position;
}

void GameObject::setSize(const sf::Vector2f& size)
{

	m_size = size;

	// Check if the texture is loaded before trying to get its size
	if (m_texture)
	{
		sf::Vector2u textureSize = m_texture->getSize();

		// Avoid division by zero if texture is empty
		if (textureSize.x > 0 && textureSize.y > 0)
		{
			float scaleX = size.x / textureSize.x;
			float scaleY = size.y / textureSize.y;
			m_sprite.setScale(scaleX, scaleY);
		}
	}
}

sf::Vector2f GameObject::getSize() const
{
	return m_size;
}

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