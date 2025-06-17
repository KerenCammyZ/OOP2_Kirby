#include "WorldMap.h"
#include "Floor.h"
#include "Wall.h"
#include <iostream>

WorldMap::WorldMap(std::shared_ptr<sf::Texture>& backgroundTexture)
	: m_backgroundTexture(backgroundTexture)
{
	// world map setup
	sf::Vector2f originalMapSize = sf::Vector2f(backgroundTexture->getSize());
	sf::Vector2f targetMapSize(SCREEN_WIDTH, SCREEN_HEIGHT);

	// Calculate the scale factors
	sf::Vector2f mapScale(targetMapSize.x / originalMapSize.x, targetMapSize.y / originalMapSize.y);
	setScale(mapScale);

	// Initialize and set the size of the visual map
	init(backgroundTexture);
	setSize(targetMapSize);
}

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

// set the size of the visual map (by scaling)
void WorldMap::setSize(const sf::Vector2f& size)
{
	// is currently called with the passed argument sf::vector2f(VIEW_WIDTH, VIEW_HEIGHT) 
	// from the constructor
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

// Ensures that the collision map aligns with the visual map.
void WorldMap::setScale(const sf::Vector2f& scale)
{
	m_scale = scale;

	// Usage:
	// (previously mapScale)
	// m_worldMap->loadCollisions(collisionImage, mapScale);
	// float scaledTileWidth = TILE_SIZE * mapScale.x;
	// float scaledTileHeight = TILE_SIZE * mapscale.y;

	// Responsibility:
	// m_scale is used to scale the collision map's tiles to match the visual map's dimensions.
	// This ensures that collisions occur at the correct positions relative to the visual representation.

	// Computed by:
	// sf::Vector2f originalMapSize = sf::Vector2f(backgroundTexture->getSize());
	// sf::Vector2f targetMapSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	// sf::Vector2f mapScale(targetMapSize.x / originalMapSize.x, targetMapSize.y / originalMapSize.y);
	// setScale(mapScale);
	//                         (the ratio of the visual map's size to the collision map's size.)
}

// Sets the collision map for the world map.
void WorldMap::setCollisionMap(std::unique_ptr<sf::Image> collisionMap)
{
	m_worldMap = std::move(collisionMap);
}

// Loads the collision map and generates collidable objects based on the colors in the image.
// This method processes a collision map(an image) to generate collidable objects(StaticObject instances)
// based on pixel colors in the image.
std::vector<std::unique_ptr<StaticObject>> WorldMap::loadCollisions()
{
	std::vector<std::unique_ptr<StaticObject>> collidables;
	if (!m_worldMap)
	{
		throw std::runtime_error("Collision map not set for WorldMap");
	}

	sf::Vector2u mapSize = m_worldMap->getSize(); // collision map size
	//const float TILE_SIZE = 1.0f; // The size of a tile in the ORIGINAL image
	const float TILE_SIZE = ENTITY_SIZE; // The size of a tile in the ORIGINAL image

	sf::Color groundColor(76, 255, 0);
	sf::Color wallColor(255, 0, 0);
	for (unsigned int y = 0; y < mapSize.y; ++y)
	{
		for (unsigned int x = 0; x < mapSize.x; ++x)
		{
			// Process ground tiles
			if (m_worldMap->getPixel(x, y) == groundColor)
			{
				auto floorTile = std::make_unique<Floor>();

				// -- calculate the position and size of each tile.
				// -- The position is adjusted to center the tile:
	
				// Calculate the SCALED size of the tile
				float scaledTileWidth = TILE_SIZE * m_scale.x;
				float scaledTileHeight = TILE_SIZE * m_scale.y;

				// Calculate the SCALED position of the tile's center
				float posX = (x * scaledTileWidth) + (scaledTileWidth / 2.f);
				float posY = (y * scaledTileHeight) + (scaledTileHeight / 2.f);

				floorTile->setPosition({ posX, posY });
				floorTile->setSize({ scaledTileWidth, scaledTileHeight });

				collidables.push_back(std::move(floorTile));
			}
			// Process wall tiles
			else if (m_worldMap->getPixel(x, y) == wallColor)
			{
				;
			}
		}
	}
	std::cout << "Generated " << collidables.size() << " collidable tiles from the collision map.\n";
	return collidables;
}