// FixedObject.cpp
#include "GameObj/FixedObj/FixedObject.h"
#include "GameObj/MovingObj/Kirby.h" // For handleCollision

FixedObject::FixedObject()
{
	// Configure the appearance of the debug shape once on creation
	m_collisionShape.setFillColor(sf::Color::Transparent);
	m_collisionShape.setOutlineColor(sf::Color::White);
	m_collisionShape.setOutlineThickness(1.f);
}

// Override: Call the base to draw the sprite (if any), then draw the debug shape
void FixedObject::draw(sf::RenderTarget& target) const
{
	GameObject::draw(target);
	//target.draw(m_collisionShape);
}

// Override: Call the base to set logical size, then set the debug shape's size
void FixedObject::setSize(const sf::Vector2f& size)
{
	GameObject::setSize(size); // Sets m_size
	m_collisionShape.setSize(size);
	m_collisionShape.setOrigin(size.x / 2.f, size.y / 2.f);
}

// Override: Call the base to set logical position, then set the debug shape's position
void FixedObject::setPosition(const sf::Vector2f& position)
{
	GameObject::setPosition(position); // Sets m_position
	m_collisionShape.setPosition(position);

	// adjust the sprite's origin and position
	if (m_texture) {
		sf::Vector2u texSize = m_texture->getSize();      // 16×16
		m_sprite.setOrigin(texSize.x / 2.f, texSize.y / 2.f);  // Origin at (8, 8)
	}
	m_sprite.setPosition(m_position);

	
}

// Override: Return the bounds of the debug shape, not the empty sprite
sf::FloatRect FixedObject::getBounds() const
{
	return m_collisionShape.getGlobalBounds();
}

// Double dispatch implementation
void FixedObject::handleCollision(GameObject* other)
{
	other->handleCollision(this);
}

void FixedObject::handleCollision(Enemy* enemy) { enemy->handleCollision(this); }