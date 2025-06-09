#include "WorldMap.h"

void WorldMap::draw(Renderer& renderer) const
{
	renderer.draw(*m_texture, sf::Vector2f(0,0), sf::Vector2f(1,1));
}
