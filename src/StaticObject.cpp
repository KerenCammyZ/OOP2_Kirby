#include "StaticObject.h"
#include "Kirby.h"

StaticObject::StaticObject()
{
	// Configure the appearance of the debug shape
	m_debugShape.setFillColor(sf::Color(120, 120, 120, 150)); // Semi-transparent grey
	m_debugShape.setOutlineColor(sf::Color::White);
	m_debugShape.setOutlineThickness(1.f);
}

void StaticObject::draw(sf::RenderTarget& target) const
{
	// Call the base class draw first (in case there's ever a texture)
	GameObject::draw(target);
	// Then, draw our debug shape
	target.draw(m_debugShape);
}

void StaticObject::setSize(const sf::Vector2f& size)
{
	GameObject::setSize(size); // Call base to handle sprite scaling
	m_debugShape.setSize(size);
	m_debugShape.setOrigin(size.x / 2.f, size.y / 2.f); // Keep origin centered
}

void StaticObject::setPosition(const sf::Vector2f& position)
{
	GameObject::setPosition(position); // Call base to update m_position
	m_debugShape.setPosition(position);
}

void StaticObject::handleCollision(GameObject* other)
{
	// Standard double dispatch entry
	other->handleCollision(this);
}
