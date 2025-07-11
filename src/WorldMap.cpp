// WorldMap.h
#include "WorldMap.h"
#include "GameObjectFactory.h"
#include "GameObj/FixedObj/Door.h"
#include "GameObj/FixedObj/Wall.h"
#include "GameObj/FixedObj/Water.h"
#include "GameObj/MovingObj/Enemy.h"
#include "GlobalSizes.h"

// Standard library includes
#include <iostream>
#include <stdexcept>
#include <map>

const sf::Color waterColor(0, 0, 200);

// Constructor: sets up the visual sprite and calculates the scale
WorldMap::WorldMap(std::shared_ptr<sf::Texture> backgroundTexture)
{
	if (!backgroundTexture)
	{
		throw std::runtime_error("Cannot create WorldMap with a null texture");
	}
	m_backgroundTexture = backgroundTexture;

	// Calculate the scale needed to fit the visual map to the screen
	sf::Vector2f originalMapSize = sf::Vector2f(m_backgroundTexture->getSize());
	sf::Vector2f targetMapSize((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT);
	m_scale = { targetMapSize.x / originalMapSize.x, targetMapSize.y / originalMapSize.y };

	// Initialize the sprite with the correct texture and scale
	init(m_backgroundTexture);
	setSize(targetMapSize);
}

// WorldMap::~WorldMap() = default;

// Initialize the background sprite
void WorldMap::init(std::shared_ptr<sf::Texture> backgroundTexture)
{
	m_backgroundSprite.setTexture(*m_backgroundTexture);
	m_backgroundSprite.setPosition(0.f, 0.f);
}

// Draw the visual background
void WorldMap::draw(sf::RenderTarget& target) const
{
	target.draw(m_backgroundSprite);
}

// This is the new, primary function for loading all objects.
// It replaces the logic that was previously in GameController.
//std::vector<std::unique_ptr<GameObject>> WorldMap::loadObjectsFromFile(const std::string& filePath, const Kirby* kirby)
//{
//	// Load the collision map image
//	sf::Image collisionImage;
//	if (!collisionImage.loadFromFile(filePath))
//	{
//		throw std::runtime_error("Failed to load collision map image: " + filePath);
//	}
//
//	std::vector<std::unique_ptr<GameObject>> createdObjects;
//	std::map<sf::Color, std::vector<sf::Vector2f>, ColorComparator> doorLocations;
//	const float TILE_SIZE = 1.0f;
//
//	// --- PASS 1: Create factory-based objects and find door locations ---
//	for (unsigned int y = 0; y < collisionImage.getSize().y; ++y)
//	{
//		for (unsigned int x = 0; x < collisionImage.getSize().x; ++x)
//		{
//			sf::Color pixelColor = collisionImage.getPixel(x, y);
//			if (pixelColor.a == 0) continue; // Skip transparent pixels
//
//			sf::Vector2f position(
//				(x * TILE_SIZE * m_scale.x) + (TILE_SIZE * m_scale.x / 2.f),
//				(y * TILE_SIZE * m_scale.y) + (TILE_SIZE * m_scale.y / 2.f) + 1.0f // + 1.0f for collision overlap
//			);
//
//			auto newObject = GameObjectFactory::create(pixelColor, position, kirby);
//
//			if (newObject)
//			{
//				newObject->setSize({ TILE_SIZE * m_scale.x, TILE_SIZE * m_scale.y });
//				newObject->setPosition(position);
//				if(newObject.get()->getType() == ObjectType::ENEMY)
//				{					
//					newObject->setSize({ ENTITY_SIZE, ENTITY_SIZE });
//				}
//				if (newObject.get()->getType() == ObjectType::SPEED_PRESENT)
//				{
//					newObject->setSize({ ENTITY_SIZE, ENTITY_SIZE });
//				}
//				createdObjects.push_back(std::move(newObject));
//			}
//			else
//			{
//				// If the factory can't create it, assume it's a door and store its location
//				doorLocations[pixelColor].push_back(position);
//			}
//		}
//	}
//
//	// --- PASS 2: Create and link the doors ---
//	for (const auto& pair : doorLocations)
//	{
//		const auto& locations = pair.second;
//		if (locations.size() == 2)
//		{
//			// Create Door A, which leads to B's location
//			auto doorA = std::make_unique<Door>(locations[1]);
//			doorA->setPosition(locations[0]);
//			doorA->setSize({ TILE_SIZE * m_scale.x * 4, TILE_SIZE * m_scale.y * 4 }); // Make doors larger
//
//			// Create Door B, which leads to A's location
//			auto doorB = std::make_unique<Door>(locations[0]);
//			doorB->setPosition(locations[1]);
//			doorB->setSize({ TILE_SIZE * m_scale.x * 4, TILE_SIZE * m_scale.y * 4 });
//
//			createdObjects.push_back(std::move(doorA));
//			createdObjects.push_back(std::move(doorB));
//		}
//		else if (!locations.empty())
//		{
//			// Warn the level designer if a door color doesn't have exactly 2 pixels
//			std::cerr << "Warning: Invalid door pair for color ("
//				<< (int)pair.first.r << "," << (int)pair.first.g << "," << (int)pair.first.b
//				<< "). Found " << locations.size() << " pixels, but expected 2.\n";
//		}
//	}
//
//	std::cout << "Loaded " << createdObjects.size() << " objects from " << filePath << "\n";
//	return createdObjects;
//}

std::vector<std::unique_ptr<GameObject>> WorldMap::loadObjectsFromFile(const std::string& filePath, const Kirby* kirby)
{
    sf::Image collisionImage;
    if (!collisionImage.loadFromFile(filePath))
    {
        throw std::runtime_error("Failed to load collision map image: " + filePath);
    }

    std::vector<std::unique_ptr<GameObject>> createdObjects;
    // This map will store locations of special, non-factory objects (doors, water markers)
    std::map<sf::Color, std::vector<sf::Vector2f>, ColorComparator> specialLocations;
    const float TILE_SIZE = 1.0f;

    // --- PASS 1: Create factory objects and find marker locations ---
    for (unsigned int y = 0; y < collisionImage.getSize().y; ++y)
    {
        for (unsigned int x = 0; x < collisionImage.getSize().x; ++x)
        {
            sf::Color pixelColor = collisionImage.getPixel(x, y);
            if (pixelColor.a == 0) continue;

            sf::Vector2f position(
                (x * TILE_SIZE * m_scale.x) + (TILE_SIZE * m_scale.x / 2.f),
                (y * TILE_SIZE * m_scale.y) + (TILE_SIZE * m_scale.y / 2.f)
            );

            // Try to create standard objects with the factory
            auto newObject = GameObjectFactory::create(pixelColor, position, kirby);

            if (newObject)
            {
                // Standard object creation...
                newObject->setSize({ TILE_SIZE * m_scale.x, TILE_SIZE * m_scale.y });
                newObject->setPosition(position);
                createdObjects.push_back(std::move(newObject));
            }
            else
            {
                // If the factory can't create it, it's a special marker (door or water)
                specialLocations[pixelColor].push_back(position);
            }
        }
    }

    // --- PASS 2: Process the special markers ---
    for (const auto& pair : specialLocations)
    {
        const auto& colorKey = pair.first;
        const auto& locations = pair.second;

        // --- NEW: Handle Water Zone Markers ---
        if (colorKey == waterColor)
        {
            //if (locations.size() == 2)
            //{
            //    sf::Vector2f p1 = locations[0];
            //    sf::Vector2f p2 = locations[1];

            //    // Calculate the bounding box
            //    float left = std::min(p1.x, p2.x);
            //    float top = std::min(p1.y, p2.y);
            //    float width = std::abs(p1.x - p2.x);
            //    float height = std::abs(p1.y - p2.y);

            //    // Create a single Water object for the whole zone
            //    auto waterZoneObject = std::make_unique<Water>();
            //    waterZoneObject->setSize({ width, height });
            //    waterZoneObject->setPosition({ left + width / 2.f, top + height / 2.f });

            //    createdObjects.push_back(std::move(waterZoneObject));
            //    std::cout << "Created Water Zone at: (" << left << "," << top << ") with size (" << width << "," << height << ")\n";
            //}
            //else {
            //    std::cerr << "Warning: Invalid water zone pair. Found " << locations.size() << " markers, but expected 2.\n";
            //}
            // First, check if there's an odd number of markers, which is an error.
            if (locations.size() % 2 != 0)
            {
                std::cerr << "Warning: Odd number of water markers found (" << locations.size()
                    << "). Each water zone requires a pair of markers. The last marker will be ignored.\n";
            }

            // Loop through the locations vector, taking 2 points at a time.
            for (size_t i = 0; i + 1 < locations.size(); i += 2)
            {
                sf::Vector2f p1 = locations[i];
                sf::Vector2f p2 = locations[i + 1];

                // Calculate the bounding box for the current pair
                float left = std::min(p1.x, p2.x);
                float top = std::min(p1.y, p2.y);
                float width = std::abs(p1.x - p2.x);
                float height = std::abs(p1.y - p2.y);

                // Create a single Water object for this zone
                auto waterZoneObject = std::make_unique<Water>();
                waterZoneObject->setSize({ width, height });
                waterZoneObject->setPosition({ left + width / 2.f, top + height / 2.f });

                createdObjects.push_back(std::move(waterZoneObject));
                std::cout << "Created Water Zone at: (" << left << "," << top << ") with size (" << width << "," << height << ")\n";
            }
        }
        // --- Handle Doors (your existing logic) ---
        else
        {
            // This is your original door logic, it can stay as is
            if (locations.size() == 2) {
                // Create Door A, which leads to B's location
			auto doorA = std::make_unique<Door>(locations[1]);
			doorA->setPosition(locations[0]);
			doorA->setSize({ TILE_SIZE * m_scale.x * 4, TILE_SIZE * m_scale.y * 4 }); // Make doors larger

			// Create Door B, which leads to A's location
			auto doorB = std::make_unique<Door>(locations[0]);
			doorB->setPosition(locations[1]);
			doorB->setSize({ TILE_SIZE * m_scale.x * 4, TILE_SIZE * m_scale.y * 4 });

			createdObjects.push_back(std::move(doorA));
			createdObjects.push_back(std::move(doorB));
            }
        }
    }

    return createdObjects;
}



// --- Getters and Setters ---

// Return the size of the world map sprite
sf::Vector2f WorldMap::getSize() const
{
	return sf::Vector2f(m_backgroundSprite.getGlobalBounds().width, m_backgroundSprite.getGlobalBounds().height);
}

// Set the size of the world map sprite based on the texture size and scale
void WorldMap::setSize(const sf::Vector2f& size)
{
	sf::Vector2u texSize = m_backgroundTexture->getSize();
	if (texSize.x > 0 && texSize.y > 0)
	{
		m_backgroundSprite.setScale(m_scale.x, m_scale.y);
	}
}

// Get the bounds of the world map sprite
sf::FloatRect WorldMap::getBounds() const
{
	return m_backgroundSprite.getGlobalBounds();
}