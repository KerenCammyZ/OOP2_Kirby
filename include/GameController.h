// GameController.h
#pragma once
#include <SFML/Graphics.hpp>
#include "Kirby.h"
#include "WorldMap.h"
#include "LevelManager.h"
#include "Camera.h"
#include "GlobalSizes.h"
#include "Floor.h"

class GameController
{
public:
	GameController();
	~GameController() = default;
	void run();
	void handleLevelTransition();
private:
	void update(float deltaTime);
	void handle();
	void draw();
	void checkCollisions();	

	sf::RenderWindow m_window;
	sf::Clock m_deltaClock;
	float m_deltaTime;

	std::shared_ptr<sf::Texture> m_kirbyTexture;
	std::unique_ptr<Kirby> m_kirby;

	std::unique_ptr<WorldMap> m_worldMap;
	std::shared_ptr<sf::Texture> m_worldMapTexture;
	std::unique_ptr<LevelManager> m_levelManager;
	std::unique_ptr<Camera> m_camera;

	// A vector to hold all static objects
	std::vector<std::unique_ptr<StaticObject>> m_staticObjects;
};