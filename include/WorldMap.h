#pragma once

#include <SFML/Graphics.hpp>
// Disable MSVC analysis warnings for the box2d include
#pragma warning(push)
#pragma warning(disable: 26495 26813)
#include "box2d/box2d.h"
#pragma warning(pop)

#include "Renderer.h"
#include "GameObject.h"

class WorldMap
{
public:
	WorldMap(b2World& world);// Default constructor
	WorldMap(sf::Image& collisionIm, sf::Texture backround, b2World& world); 
	~WorldMap() = default;
	void draw(Renderer& renderer) const;
	void loadFromImage(const sf::Image& image);
private:
	b2World& m_world; // Reference to the Box2D world
	sf::RectangleShape m_shape; // Shape representing the world map
	std::shared_ptr<sf::Texture> m_texture;
	sf::Image m_collisionMap;
	//sf::Texture m_backgroundTexture;
	std::vector<std::vector<std::unique_ptr<GameObject>>> m_grid;
};