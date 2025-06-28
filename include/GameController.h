// GameController.h
#pragma once
#include <SFML/Graphics.hpp>
#include "GameObj/MovingObj/Kirby.h"
#include "GameObj/MovingObj/Enemy.h"
#include "GameObj/FixedObj/Floor.h"
#include "GameObj/FixedObj/Wall.h"
#include "GameObj/FixedObj/Present.h"
#include "WorldMap.h"
#include "GlobalSizes.h"
#include <vector>

class GameController
{
public:
	GameController();
	~GameController() = default;
	void run();

private:
	void update(float deltaTime);
	void handle();
	void draw();
	void checkCollisions();
	void updateView();

	void loadTextures();
	void loadCollisionMap(std::string collisionMap);

	float m_levelBlockHeight;
	float m_deltaTime;
	sf::Clock m_deltaClock;
	sf::View m_view;
	sf::RenderWindow m_window;

	std::unique_ptr<Kirby> m_kirby;
	std::unique_ptr<WorldMap> m_worldMap;

	std::shared_ptr<sf::Texture> m_kirbyTexture;
	std::shared_ptr<sf::Texture> m_worldMapTexture;

	std::vector<std::unique_ptr<GameObject>> m_allGameObjects;
	std::vector<std::unique_ptr<Enemy>> m_enemies;
};