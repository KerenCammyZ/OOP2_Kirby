#include "WorldMap.h"
#include "GameObj/FixedObj/Floor.h"
#include "GameObj/FixedObj/Wall.h"
#include "GameObj/FixedObj/Door.h"
#include <iostream> // for debugging
#include <map>
#include <vector>

// --- FIX: Define a custom comparator struct for sf::Color ---
// This struct provides the sorting rules for the std::map.
// It's a self-contained way to tell the map how to order its keys.
struct ColorComparator {
	bool operator()(const sf::Color& a, const sf::Color& b) const {
		if (a.r != b.r) return a.r < b.r;
		if (a.g != b.g) return a.g < b.g;
		if (a.b != b.b) return a.b < b.b;
		return a.a < b.a;
	}
};

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

// Initialize the world map by setting the background graphics
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
	std::cout << "WorldMap::getSize() no m_backgroundTexture"; // FOR DEBUGGING
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
		if (texSize.x > 0 && texSize.y > 0) {
			float scaleX = size.x / static_cast<float>(texSize.x);
			float scaleY = size.y / static_cast<float>(texSize.y);
			m_backgroundSprite.setScale(scaleX, scaleY);
		}
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
// This method processes a collision map(an image) to generate collidable objects(FixedObject instances)
// based on pixel colors in the image.
std::vector<std::unique_ptr<FixedObject>> WorldMap::loadCollisions()
{
	std::vector<std::unique_ptr<FixedObject>> collidables;
	if (!m_worldMap) { throw std::runtime_error("Collision map not set for WorldMap"); }

	sf::Vector2u mapSize = m_worldMap->getSize(); // collision map size
	const float TILE_SIZE = 1.0f; // The size of a tile in the ORIGINAL image

	sf::Color groundColor(76, 255, 0);
	sf::Color wallColor(255, 0, 0);
	const std::vector<sf::Color> doorPairColors = {
		sf::Color(0, 38, 255),       // Door Color A
		sf::Color(0, 148, 255),   // Door Color B
	};

	// --- FIX: Use the custom comparator in the map definition ---Add commentMore actions
	std::map<sf::Color, std::vector<sf::Vector2f>, ColorComparator> doorLocations;

	// --- PASS 1: Find all special pixels and categorize them ---

	for (unsigned int y = 0; y < mapSize.y; ++y)
	{
		for (unsigned int x = 0; x < mapSize.x; ++x)
		{
			sf::Color pixelColor = m_worldMap->getPixel(x, y);

			float posX = (x * TILE_SIZE * m_scale.x) + (TILE_SIZE * m_scale.x / 2.f);
			float posY = (y * TILE_SIZE * m_scale.y) + (TILE_SIZE * m_scale.y / 2.f);
			sf::Vector2f position(posX, posY);

			// Process ground tiles
			if (pixelColor == groundColor)
			{
				auto floorTile = std::make_unique<Floor>();

				floorTile->setPosition(position);
				floorTile->setSize({ TILE_SIZE * m_scale.x, TILE_SIZE * m_scale.y });
				collidables.push_back(std::move(floorTile));
			}
			// Process wall tiles
			else if (pixelColor == wallColor)
			{
				;
			}
			else
			{
				for (const auto& doorColor : doorPairColors)
				{
					if (pixelColor == doorColor)
					{
						doorLocations[doorColor].push_back(position);
						break;
					}
				}
			}
		}
	}
	// --- PASS 2: Process the door map and create linked pairs ---
	for (const auto& pair : doorLocations)
	{
		const std::vector<sf::Vector2f>& locations = pair.second;
		if (locations.size() == 2)
		{
			sf::Vector2f posA = locations[0];
			sf::Vector2f posB = locations[1];

			auto doorA = std::make_unique<Door>(posB);
			doorA->setPosition(posA);
			doorA->setSize({ TILE_SIZE * m_scale.x, TILE_SIZE * m_scale.y * 2.f });

			auto doorB = std::make_unique<Door>(posA);
			doorB->setPosition(posB);
			doorB->setSize({ TILE_SIZE * m_scale.x, TILE_SIZE * m_scale.y * 2.f });

			collidables.push_back(std::move(doorA));
			collidables.push_back(std::move(doorB));

			std::cout << "Created a door pair between (" << posA.x << "," << posA.y << ") and (" << posB.x << "," << posB.y << ")\n";
		}
		else
		{
			std::cerr << "Warning: Invalid door pair found! Expected 2 pixels for a door, but found "
				<< locations.size() << ". Check your collision map.\n";
		}
	}
	std::cout << "Generated " << collidables.size() << " collidable tiles from the collision map.\n";
	return collidables;
}

/*
// In WorldMap.cpp - Simple collision loading
std::vector<std::unique_ptr<FixedObject>> WorldMap::loadCollisions()
{
    std::vector<std::unique_ptr<FixedObject>> collidables;
    if (!m_worldMap) return collidables;

    sf::Vector2u mapSize = m_worldMap->getSize();
    sf::Color groundColor(76, 255, 0);
    
    for (unsigned int y = 0; y < mapSize.y; ++y)
    {
        for (unsigned int x = 0; x < mapSize.x; ++x)
        {
            if (m_worldMap->getPixel(x, y) == groundColor)
            {
                auto floorTile = std::make_unique<Floor>();
                
                // Position tile at center of the scaled pixel area
                float posX = (x * m_scale.x) + (m_scale.x / 2.f);
                float posY = (y * m_scale.y) + (m_scale.y / 2.f);
                floorTile->setPosition({posX, posY});
                floorTile->setSize({m_scale.x, m_scale.y});
                
                collidables.push_back(std::move(floorTile));
            }
        }
    }
    
    std::cout << "Generated " << collidables.size() << " collision tiles." << std::endl;
    return collidables;
}
*/