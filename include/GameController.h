#pragma once
#include <SFML/Graphics.hpp>
#include "Kirby.h"
#include "WorldMap.h"
#include "GlobalSizes.h"
#include "Floor.h"

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

	sf::RenderWindow m_window;
	sf::Clock m_deltaClock;
	float m_deltaTime;

	std::shared_ptr<sf::Texture> m_kirbyTexture;
	std::unique_ptr<Kirby> m_kirby;

	std::shared_ptr<sf::Texture> m_worldMapTexture;
	std::unique_ptr<WorldMap> m_worldMap;

	// A vector to hold all static objects
	std::vector<std::unique_ptr<StaticObject>> m_staticObjects;
};