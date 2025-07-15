// FixedObject.cpp
#include "GameObj/FixedObj/FixedObject.h"
#include "GameObj/MovingObj/Kirby.h"

FixedObject::FixedObject()
{
	m_collisionShape.setFillColor(sf::Color::Transparent);
	m_collisionShape.setOutlineColor(sf::Color::White);
	m_collisionShape.setOutlineThickness(1.f);
}

void FixedObject::draw(sf::RenderTarget& target) const
{
	GameObject::draw(target);
}

void FixedObject::setSize(const sf::Vector2f& size)
{
	GameObject::setSize(size);
	m_collisionShape.setSize(size);
	m_collisionShape.setOrigin(size.x / 2.f, size.y / 2.f);
}

void FixedObject::setPosition(const sf::Vector2f& position)
{
	GameObject::setPosition(position);
	m_collisionShape.setPosition(position);
}

sf::FloatRect FixedObject::getBounds() const
{
	return m_collisionShape.getGlobalBounds();
}

void FixedObject::handleCollision(GameObject* other)
{
	other->handleCollision(this);
}

void FixedObject::handleCollision(Enemy* enemy)
{ 
	enemy->handleCollision(this);
}