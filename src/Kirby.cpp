#include "Kirby.h"

Kirby::Kirby() {};

void Kirby::move(float deltaTime)
{
	b2Vec2 vel = getVelocity();

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		vel.x = -m_speed;
	} else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		vel.x = m_speed;
	} else {
		vel.x = 0.0f;
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		vel.y = -m_speed;
	} else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		vel.y = m_speed;
	} else {
		vel.y = 0.0f;
	}
	setVelocity(vel);
	
	m_sprite.setOrigin(m_texture->getSize().x / 2.f, m_texture->getSize().y / 2.f);
	m_sprite.setPosition(m_position);
	m_sprite.setScale(1.0f, 1.0f); // Use texture's natural size
}