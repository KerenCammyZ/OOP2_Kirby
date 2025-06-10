#pragma once
#include <string>
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
	WorldMap(); // Default constructor
	~WorldMap() = default;
	
	void draw(sf::RenderTarget& target) const;
	void loadCollisionImage(const std::string& fileName);
	void loadBackground(const std::string& fileName);

private:
	//b2World m_world; // Box2D world for physics simulation
	std::unique_ptr<sf::Texture> m_backroundMap;
	std::unique_ptr<sf::Image> m_collisionMap;
};