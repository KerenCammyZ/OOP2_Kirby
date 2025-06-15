#include "WorldMap.h"
#include "Floor.h"
#include "Door.h"
#include <iostream>
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

WorldMap::WorldMap() {}

WorldMap::~WorldMap() = default;

void WorldMap::init(std::shared_ptr<sf::Texture> backgroundTexture)
{
	if (!backgroundTexture)
		throw std::runtime_error("Background texture cannot be null");

	m_backgroundTexture = backgroundTexture;
	m_backgroundSprite.setTexture(*m_backgroundTexture);
	m_backgroundSprite.setPosition(0.f, 0.f);
}

void WorldMap::draw(sf::RenderTarget& target) const
{
	if (m_backgroundTexture) {
		target.draw(m_backgroundSprite);
	}
	else {
		throw std::runtime_error("Background texture not set for WorldMap");
	}
}

sf::Vector2f WorldMap::getSize() const
{
	if (m_backgroundTexture) {
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
	if (m_backgroundTexture) {
		sf::Vector2u texSize = m_backgroundTexture->getSize();
		if (texSize.x > 0 && texSize.y > 0) {
			float scaleX = size.x / static_cast<float>(texSize.x);
			float scaleY = size.y / static_cast<float>(texSize.y);
			m_backgroundSprite.setScale(scaleX, scaleY);
		}
	}
	else {
		throw std::runtime_error("Background texture not set for WorldMap");
	}
}


std::vector<std::unique_ptr<StaticObject>> WorldMap::loadCollisions(const sf::Image& collisionMap, sf::Vector2f scale)
{
	std::vector<std::unique_ptr<StaticObject>> collidables;
	sf::Vector2u mapSize = collisionMap.getSize();
	const float TILE_SIZE = 1.0f;

	const sf::Color groundColor(76, 255, 0);
	const std::vector<sf::Color> doorPairColors = {
		sf::Color(0, 38, 255),       // Door Color A
		sf::Color(0, 148, 255),   // Door Color B
	};

	// --- FIX: Use the custom comparator in the map definition ---
	std::map<sf::Color, std::vector<sf::Vector2f>, ColorComparator> doorLocations;

	// --- PASS 1: Find all special pixels and categorize them ---
	for (unsigned int y = 0; y < mapSize.y; ++y)
	{
		for (unsigned int x = 0; x < mapSize.x; ++x)
		{
			sf::Color pixelColor = collisionMap.getPixel(x, y);

			float posX = (x * TILE_SIZE * scale.x) + (TILE_SIZE * scale.x / 2.f);
			float posY = (y * TILE_SIZE * scale.y) + (TILE_SIZE * scale.y / 2.f);
			sf::Vector2f position(posX, posY);

			if (pixelColor == groundColor)
			{
				auto floorTile = std::make_unique<Floor>();
				floorTile->setPosition(position);
				floorTile->setSize({ TILE_SIZE * scale.x, TILE_SIZE * scale.y });
				collidables.push_back(std::move(floorTile));
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
			doorA->setSize({ TILE_SIZE * scale.x, TILE_SIZE * scale.y * 2.f });

			auto doorB = std::make_unique<Door>(posA);
			doorB->setPosition(posB);
			doorB->setSize({ TILE_SIZE * scale.x, TILE_SIZE * scale.y * 2.f });

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

	return collidables;
}