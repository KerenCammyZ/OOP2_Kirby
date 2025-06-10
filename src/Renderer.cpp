#include "Renderer.h"

void Renderer::draw(const sf::Texture& texture, const sf::Vector2f& position)
{
	m_sprite.setTexture(texture);
	m_sprite.setOrigin(texture.getSize().x / 2.f, texture.getSize().y / 2.f);
	m_sprite.setPosition(position);
	m_sprite.setScale(1.0f, 1.0f); // Use texture's natural size
	m_target.draw(m_sprite);
}

void Renderer::draw(const sf::Texture& texture, const sf::Vector2f& position, const sf::Vector2f& size)
{
	m_sprite.setTexture(texture);
	m_sprite.setOrigin(texture.getSize().x / 2.f, texture.getSize().y / 2.f);
	m_sprite.setPosition(position);
	m_sprite.setScale(size.x / texture.getSize().x, size.y / texture.getSize().y);
	m_target.draw(m_sprite);
}