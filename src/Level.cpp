#include "Level.h"
#include "ResourceManager.h"
#include "GameObj/MovingObj/Enemy.h" // Ensure Enemy is included for dynamic_cast

Level::Level(int levelNumber, Kirby* kirby)
	: levelNumber(levelNumber), complete(false)
{
	collisionMap = "Level" + std::to_string(levelNumber) + "Collisions.png";
	backgroundImage = "Level" + std::to_string(levelNumber) + ".png";

	m_worldMapTexture = ResourceManager::getInstance().getTexture(backgroundImage);
	if (!m_worldMapTexture)
	{
		throw std::runtime_error("Failed to load world map texture: " + backgroundImage);
	}

	m_worldMap = std::make_unique<WorldMap>(m_worldMapTexture);

	// Load all the objects associated with this level
	loadObjects(kirby);
}

void Level::loadObjects( Kirby* kirby)
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


std::vector<std::unique_ptr<GameObject>> Level::getObjects()
{
	return std::move(m_objects);
}

std::vector<std::unique_ptr<Enemy>> Level::getEnemies()
{
	return std::move(m_enemies);
}

// transfer ownership of the WorldMap
std::unique_ptr<WorldMap> Level::getWorldMap()
{
	return std::move(m_worldMap);
}

bool Level::getCompleted() const
{
	if (this)
		return complete;
	else
		return false;
}

void Level::setCompleted(bool completed)
{
	complete = completed;
}