#include "WorldMap.h"
#include "Floor.h"
#include "Wall.h"
#include <iostream>

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

void WorldMap::setSize(const sf::Vector2f& size)
{
	if (m_backgroundTexture)
	{
		sf::Vector2u texSize = m_backgroundTexture->getSize();
		float scaleX = size.x / static_cast<float>(texSize.x);
		float scaleY = size.y / static_cast<float>(texSize.y);
		m_backgroundSprite.setScale(scaleX, scaleY);
	}
	else
	{
		throw std::runtime_error("Background texture not set for WorldMap");
	}
}

std::vector<std::unique_ptr<StaticObject>> WorldMap::loadCollisions(const sf::Image& collisionMap ,sf::Vector2f scale)
{
	std::vector<std::unique_ptr<StaticObject>> collidables;
	sf::Vector2u mapSize = collisionMap.getSize();
	const float TILE_SIZE = 1.0f; // The size of a tile in the ORIGINAL image
	sf::Color groundColor(76, 255, 0);
	sf::Color wallColor(255, 0, 0);
	for (unsigned int y = 0; y < mapSize.y; ++y)
	{
		for (unsigned int x = 0; x < mapSize.x; ++x)
		{
			if (collisionMap.getPixel(x, y) == groundColor)
			{
				auto floorTile = std::make_unique<Floor>();

				// Calculate the SCALED size of the tile
				float scaledTileWidth = TILE_SIZE * scale.x;
				float scaledTileHeight = TILE_SIZE * scale.y;

				// Calculate the SCALED position of the tile's center
				float posX = (x * scaledTileWidth) + (scaledTileWidth / 2.f);
				float posY = (y * scaledTileHeight) + (scaledTileHeight / 2.f);

				floorTile->setPosition({ posX, posY });
				floorTile->setSize({ scaledTileWidth, scaledTileHeight });

				collidables.push_back(std::move(floorTile));
			}
			else if (collisionMap.getPixel(x, y) == wallColor)
			{
				auto wallTile = std::make_unique<Wall>();

				// Calculate the SCALED size of the tile
				float scaledTileWidth = TILE_SIZE * scale.x;
				float scaledTileHeight = TILE_SIZE * scale.y;

				// Calculate the SCALED position of the tile's center
				float posX = (x * scaledTileWidth) + (scaledTileWidth / 2.f);
				float posY = (y * scaledTileHeight) + (scaledTileHeight / 2.f);

				wallTile->setPosition({ posX, posY });
				wallTile->setSize({ scaledTileWidth, scaledTileHeight });

				collidables.push_back(std::move(wallTile));
			}
		}
	}

	std::cout << "Generated " << collidables.size() << " collidable tiles from the collision map.\n";
	return collidables;
}