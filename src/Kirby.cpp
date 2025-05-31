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
}