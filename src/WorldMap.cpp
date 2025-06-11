#include "WorldMap.h"

WorldMap::WorldMap() {}

WorldMap::~WorldMap() = default;

void WorldMap::init(std::shared_ptr<sf::Texture> backgroundTexture)
{
	if (!backgroundTexture)
		throw std::runtime_error("Background texture cannot be null");

	m_backgroundTexture = backgroundTexture;

	// Set the sprite's texture and position it at the world origin
	m_backgroundSprite.setTexture(*m_backgroundTexture);
	m_backgroundSprite.setPosition(0.f, 0.f);
}

// Updated to draw the member sprite
void WorldMap::draw(sf::RenderTarget& target) const
{
	if (m_backgroundTexture)
	{
		target.draw(m_backgroundSprite);
	}
	else
	{
		throw std::runtime_error("Background texture not set for WorldMap");
	}
}

sf::Vector2f WorldMap::getSize() const
{
	if (m_backgroundTexture)
	{
		sf::Vector2u texSize = m_backgroundTexture->getSize();
		return sf::Vector2f(static_cast<float>(texSize.x), static_cast<float>(texSize.y));
	}
	return sf::Vector2f(0.f, 0.f);
}

sf::FloatRect WorldMap::getBounds() const
{
	return m_backgroundSprite.getGlobalBounds();
}