#include "Kirby.h"

Kirby::Kirby() {};

void Kirby::move(float deltaTime)
{

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) 
	{
		m_sprite.move(-m_speed * deltaTime, 0.0f);
	} 
	else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) 
	{
		m_sprite.move(m_speed * deltaTime, 0.0f);
	} 
	else 
	{
		m_sprite.move(0.0f, 0.0f);
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) 
	{
		m_sprite.move(0.0f, -m_speed * deltaTime);
	}
	else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) 
	{
		m_sprite.move(0.0f, m_speed * deltaTime);
	}
	else 
	{
		m_sprite.move(0.0f, 0.0f);
	}

	m_sprite.setOrigin(m_texture->getSize().x / 2.f, m_texture->getSize().y / 2.f);
	m_sprite.setPosition(m_position);
}