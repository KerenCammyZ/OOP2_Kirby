// Level.h
#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include "WorldMap.h"
#include "GameObj/MovingObj/Enemy.h"
#include <iostream>
#include <memory>

class Level
{
public:
	Level(int levelNumber, Kirby* kirby);
	~Level() = default;

	std::vector<std::unique_ptr<GameObject>> getObjects();
	std::vector<std::unique_ptr<Enemy>> getEnemies();
	std::unique_ptr<WorldMap> getWorldMap();
	bool getCompleted() const;
	void setCompleted(bool completed);

private:
	void loadObjects(Kirby* kirby);
	bool m_complete;
	int m_levelNumber;
	std::string m_collisionMap;
	std::string m_backgroundImage;

	std::unique_ptr<WorldMap> m_worldMap;
	std::shared_ptr<sf::Texture> m_worldMapTexture;
	std::vector<std::unique_ptr<GameObject>> m_objects;
	std::vector<std::unique_ptr<Enemy>> m_enemies;
};
