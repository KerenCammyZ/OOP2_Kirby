#include "Level.h"
#include "GameObj/MovingObj/Enemy.h" // Ensure Enemy is included for dynamic_cast

Level::Level(int levelNumber, const Kirby* kirby) : levelNumber(levelNumber), complete(false)
{
	collisionMap = "Level" + std::to_string(levelNumber) + "Collisions.png";
	backgroundImage = "Level" + std::to_string(levelNumber) + ".png";

	// --- FIX 1: Initialize the shared_ptr before using it ---
	m_worldMapTexture = std::make_shared<sf::Texture>();
	if (!m_worldMapTexture->loadFromFile(backgroundImage))
	{
		throw std::runtime_error("Failed to load world map texture: " + backgroundImage);
	}

	// Now that the texture is valid, create the WorldMap
	m_worldMap = std::make_unique<WorldMap>(m_worldMapTexture);

	// Load all the objects associated with this level
	loadObjects(kirby);
}

void Level::loadObjects(const Kirby* kirby)
{
	// Load all objects from the collision map file
	auto objects = m_worldMap->loadObjectsFromFile(collisionMap, kirby);

	// Separate enemies from fixed objects (load enemies)
	for (auto it = objects.begin(); it != objects.end(); )
	{
		if (auto enemy = dynamic_cast<Enemy*>(it->get()))
		{
			// Move the enemy from the temp vector to our member vector
			m_enemies.push_back(std::unique_ptr<Enemy>(static_cast<Enemy*>(it->release())));
			it = objects.erase(it);
		}
		else {
			++it;
		}
	}
	// Move the remaining fixed objects to our member vector
	m_objects = std::move(objects);
	std::cout << "Loaded " << m_objects.size() << " fixed objects.\n";
	std::cout << "Loaded " << m_enemies.size() << " enemies.\n";
}

// --- FIX 2: Use std::move to transfer ownership of the vectors ---
std::vector<std::unique_ptr<GameObject>>&& Level::getObjects()
{
	return std::move(m_objects);
}

std::vector<std::unique_ptr<Enemy>>&& Level::getEnemies()
{
	return std::move(m_enemies);
}

// This function correctly transfers ownership of the WorldMap
std::unique_ptr<WorldMap> Level::getWorldMap()
{
	return std::move(m_worldMap);
}

bool Level::getCompleted() const
{
	return complete;
}

void Level::setCompleted(bool completed)
{
	complete = completed;
}