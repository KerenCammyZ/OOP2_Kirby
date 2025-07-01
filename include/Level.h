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
	Level(int levelNumber, const Kirby* kirby);
	~Level() = default;

	//std::vector<std::unique_ptr<GameObject>>&& getObjects(); // Q: perhaps better to return by value
	//std::vector<std::unique_ptr<Enemy>>&& getEnemies();      //    instead of by reference?
	std::vector<std::unique_ptr<GameObject>> getObjects(); // Q: perhaps better to return by value
	std::vector<std::unique_ptr<Enemy>> getEnemies();      //    instead of by reference?
	std::unique_ptr<WorldMap> getWorldMap();
	bool getCompleted() const;
	void setCompleted(bool completed);
private:

	void loadObjects(const Kirby* kirby);
	bool complete;  // TODO: by course conventions, class data members are prefixed 'm_'
	int levelNumber;
	std::string collisionMap;
	std::string backgroundImage;

	std::unique_ptr<WorldMap> m_worldMap;
	std::shared_ptr<sf::Texture> m_worldMapTexture;
	std::vector<std::unique_ptr<GameObject>> m_objects;
	std::vector<std::unique_ptr<Enemy>> m_enemies;
};
