// Renderer.h : Handles drawing textures and sprites to the screen.
#pragma once
#include <SFML/Graphics.hpp>

class Renderer
{
public:
	Renderer(sf::RenderTarget& target) : m_target(target) {}
	void draw(const sf::Texture& texture, const sf::Vector2f& position, const sf::Vector2f& size);
private:
	sf::Sprite m_sprite;
	sf::RenderTarget& m_target;
};