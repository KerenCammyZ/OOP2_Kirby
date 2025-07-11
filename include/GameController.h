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
#include <memory>

class GameState;

class GameController
{
public:
	GameController();
	~GameController();
	void run();
	void changeGameState(std::unique_ptr<GameState> newState);
	void addScore(unsigned int);
	Level* getLevel();
	sf::RenderWindow& getWindow();
	unsigned int getScore() const;
	void update(float deltaTime);
	void handleEvents();
	void draw();
	void loadLevel(int levelNum);
	void loadHUD();

private:
	void processWindowEvents();
	void drawHUD();
	void updateView();
	void checkCollisions();
	//void loadTextures();

	float m_deltaTime;
	sf::Clock m_deltaClock;

	int m_score = 0;
	int m_currentLevel;
	const int m_maxLevels = 3;

	sf::View m_gameView;
	sf::View m_hudView;
	sf::RenderWindow m_window;
	float m_levelAreaHeight;

	std::unique_ptr<HUD> m_hud;
	std::unique_ptr<Level> m_level;
	std::unique_ptr<Kirby> m_kirby;
	std::unique_ptr<WorldMap> m_worldMap;
	std::unique_ptr<GameState> m_currentState;

	std::vector<std::unique_ptr<Enemy>> m_enemies;
	std::vector<std::unique_ptr<GameObject>> m_fixedObjects;
};