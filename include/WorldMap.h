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
	void draw(Renderer& renderer) const;
	//void createCollisionBodies(const sf::Image& image);
	//void setTexture(std::shared_ptr<sf::Texture> texture);
	sf::Vector2f getSize() const;
	sf::FloatRect getBounds() const;

private:
	std::shared_ptr<sf::Texture> m_backgroundTexture;
	sf::Vector2f m_position; // Usually (0,0) for background
	sf::Vector2f m_size;     // Size based on texture
};