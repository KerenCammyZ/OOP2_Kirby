// WorldMap.h
#include "WorldMap.h"
#include "GameObjectFactory.h"
#include "GameObj/FixedObj/Door.h"
//#include "GameObj/FixedObj/Wall.h"
#include "GameObj/MovingObj/Enemy.h"
#include "GlobalSizes.h"

// Standard library includes
#include <iostream>
#include <stdexcept>
#include <map>

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

//WorldMap::~WorldMap() = default;

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
std::vector<std::unique_ptr<GameObject>> WorldMap::loadObjectsFromFile(const std::string& filePath)
{
	sf::Image collisionImage;
	if (!collisionImage.loadFromFile(filePath))
	{
		throw std::runtime_error("Failed to load collision map image: " + filePath);
	}

	std::vector<std::unique_ptr<GameObject>> createdObjects;
	std::map<sf::Color, std::vector<sf::Vector2f>, ColorComparator> doorLocations;
	const float TILE_SIZE = 1.0f;

	// --- PASS 1: Create factory-based objects and find door locations ---
	for (unsigned int y = 0; y < collisionImage.getSize().y; ++y)
	{
		for (unsigned int x = 0; x < collisionImage.getSize().x; ++x)
		{
			sf::Color pixelColor = collisionImage.getPixel(x, y);
			if (pixelColor.a == 0) continue; // Skip transparent pixels

			sf::Vector2f position(
				(x * TILE_SIZE * m_scale.x) + (TILE_SIZE * m_scale.x / 2.f),
				(y * TILE_SIZE * m_scale.y) + (TILE_SIZE * m_scale.y / 2.f) + 1.0f // + 1.0f for collision overlap
			);

			auto newObject = GameObjectFactory::instance().create(pixelColor, position);

			if (newObject)
			{
				newObject->setSize({ TILE_SIZE * m_scale.x, TILE_SIZE * m_scale.y });
				newObject->setPosition(position);
				createdObjects.push_back(std::move(newObject));
			}
			else
			{
				// If the factory can't create it, assume it's a door and store its location
				doorLocations[pixelColor].push_back(position);
			}
		}
	}

	// --- PASS 2: Create and link the doors ---
	for (const auto& pair : doorLocations)
	{
		const auto& locations = pair.second;
		if (locations.size() == 2)
		{
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
		else if (!locations.empty())
		{
			// Warn the level designer if a door color doesn't have exactly 2 pixels
			std::cerr << "Warning: Invalid door pair for color ("
				<< (int)pair.first.r << "," << (int)pair.first.g << "," << (int)pair.first.b
				<< "). Found " << locations.size() << " pixels, but expected 2.\n";
		}
	}

	std::cout << "Loaded " << createdObjects.size() << " objects from " << filePath << "\n";
	return createdObjects;
}


// --- Getters and Setters ---

sf::Vector2f WorldMap::getSize() const
{
	return sf::Vector2f(m_backgroundSprite.getGlobalBounds().width, m_backgroundSprite.getGlobalBounds().height);
}

sf::FloatRect WorldMap::getBounds() const
{
	return m_backgroundSprite.getGlobalBounds();
}

void WorldMap::setSize(const sf::Vector2f& size)
{
	sf::Vector2u texSize = m_backgroundTexture->getSize();
	if (texSize.x > 0 && texSize.y > 0)
	{
		m_backgroundSprite.setScale(m_scale.x, m_scale.y);
	}
}