#include "GameObject.h"

GameObject::GameObject() {}

GameObject::~GameObject() {}

void GameObject::draw(Renderer& renderer) const
{
	if (m_texture)
		renderer.draw(*m_texture, m_position, m_size);
	else
		throw std::runtime_error("Texture not set for GameObject, cannot draw.");
}

void GameObject::setPosition(const sf::Vector2f& position)
{
	m_position = position;
	if(m_body)
		m_body->SetTransform(b2Vec2(position.x / 30.0f, position.y / 30.0f), m_body->GetAngle());
	else
		throw std::runtime_error("Body not set for GameObject, cannot set position.");
}

sf::Vector2f GameObject::getPosition() const
{
	return m_position;
}

void GameObject::setSize(const sf::Vector2f& size)
{
	m_size = size;
	if (m_body)
	{
		b2PolygonShape shape;
		shape.SetAsBox(size.x / 2.0f, size.y / 2.0f);
		m_body->CreateFixture(&shape, 1.0f);
	}
	else 
		throw std::runtime_error("Body not set for GameObject, cannot set size.");
}

sf::Vector2f GameObject::getSize() const
{
	return m_size;
}

void GameObject::setTexture(std::shared_ptr<sf::Texture> texture)
{
	m_texture = texture;
}

sf::FloatRect GameObject::getBounds() const
{
	if(m_texture)
		return sf::FloatRect(m_position, m_size);
	else
		throw std::runtime_error("Texture not set for GameObject, cannot get bounds.");
}