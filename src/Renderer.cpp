#include "Renderer.h"

void Renderer::draw(const sf::Texture& texture, const sf::Vector2f& position, const sf::Vector2f& size)
{
	m_sprite.setTexture(texture);
	m_sprite.setPosition(position);
	m_sprite.setScale(size.x / texture.getSize().x, size.y / texture.getSize().y);
	m_target.draw(m_sprite);
}