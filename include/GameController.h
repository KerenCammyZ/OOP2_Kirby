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
#include "Level.h"
#include "HUD.h"
#include "SpriteSheet.h"
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
	void drawHUD();
	void checkCollisions();
	void updateView();
	void loadTextures();
	void loadLevel(int levelNum);

	float m_levelAreaHeight;
	float m_deltaTime;

	int m_currentLevel;
	const int m_maxLevels = 2;

	sf::Clock m_deltaClock;
	//sf::View m_view;
	sf::View m_gameView;
	sf::View m_hudView;
	sf::RenderWindow m_window;

	std::unique_ptr<Kirby> m_kirby;
	std::unique_ptr<WorldMap> m_worldMap;
	std::unique_ptr<Level> m_level;
	std::unique_ptr<HUD> m_hud;

	std::shared_ptr<sf::Texture> m_kirbyTexture;

	std::vector<std::unique_ptr<GameObject>> m_allGameObjects;
	std::vector<std::unique_ptr<Enemy>> m_enemies;
};