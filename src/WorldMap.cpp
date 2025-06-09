#include "WorldMap.h"

WorldMap::WorldMap(sf::Image& collisionIm, sf::Texture backround, b2World& world)
	: m_collisionMap(collisionIm) , m_texture(&backround), m_world(world) 
{
	loadFromImage(m_collisionMap);
}

void WorldMap::draw(Renderer& renderer) const
{
	renderer.draw(*m_texture, sf::Vector2f(0,0), sf::Vector2f(1,1));
}

void WorldMap::loadFromImage(const sf::Image& image)
{
	sf::Vector2u size = image.getSize();
	m_grid.resize(size.y);
	for (unsigned int i = 0; i < size.y; ++i)
	{
		m_grid[i].resize(size.x);
	}

	for (unsigned int y = 0; y < size.y; ++y)
	{
		for (unsigned int x = 0; x < size.x; ++x)
		{
			// if green create Ground GameObject

			// if red create Boundry GameObject
		}
	}
}
