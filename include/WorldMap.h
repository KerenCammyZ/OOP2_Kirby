#pragma once

#include <SFML/Graphics.hpp>
// Disable MSVC analysis warnings for the box2d include
#pragma warning(push)
#pragma warning(disable: 26495 26813)
#include "box2d/box2d.h"
#pragma warning(pop)

class WorldMap
{
public:
	WorldMap() = default; // Default constructor
	//WorldMap(b2World& world, const sf::Vector2f& size, const sf::Vector2f& position = {0.f, 0.f});
	~WorldMap() = default;
	//// Disable copy and move semantics
	//WorldMap(const WorldMap&) = delete;
	//WorldMap& operator=(const WorldMap&) = delete;
	//WorldMap(WorldMap&&) = delete;
	//WorldMap& operator=(WorldMap&&) = delete;
	//// Update the world map
	//void update(float dt);
	void draw(sf::RenderTarget& target) const;
	void loadFromImage(const sf::Image& image);
private:
	//b2World& m_world; // Reference to the Box2D world
	//sf::RectangleShape m_shape; // Shape representing the world map
	//sf::Vector2f m_size; // Size of the world map
	//sf::Vector2f m_position; // Position of the world map
	//bool m_isVisible; // Visibility flag for the world map
};