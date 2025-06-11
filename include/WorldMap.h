#pragma once

#include <SFML/Graphics.hpp>
// Disable MSVC analysis warnings for the box2d include
#pragma warning(push)
#pragma warning(disable: 26495 26813)
#include "box2d/box2d.h"
#pragma warning(pop)

#include "Renderer.h"

class WorldMap
{
public:
	WorldMap();
	~WorldMap();

	void init(std::shared_ptr<sf::Texture> backgroundTexture);
	void draw(sf::RenderTarget& target) const;
	sf::Vector2f getSize() const;
	sf::FloatRect getBounds() const;

private:
	std::shared_ptr<sf::Texture> m_backgroundTexture;
	sf::Sprite m_backgroundSprite;
};