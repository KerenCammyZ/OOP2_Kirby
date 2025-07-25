// GameController.cpp
#include "GameController.h"
#include "States/GameStates/MainMenuState.h"
#include "States/GameStates/PlayingState.h"
#include "States/KirbyStates/KirbyFallingState.h"
#include "ResourceManager.h"
#include <iostream> // for debugging

GameController::GameController():
	m_window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Kirby"), m_currentLevel(1), m_score(0)
{
	auto& rm = ResourceManager::getInstance();
	
	// Set the initial state to the Main Menu
	m_currentState = std::make_unique<MainMenuState>(*this);

	auto kirbyTexture = rm.getTexture("KirbySprite.png");
	m_kirby = std::make_unique<Kirby>(kirbyTexture);
	loadHUD(); // Load the HUD and set up the views for the game and HUD
}

void GameController::run()
{
	while (m_window.isOpen())
	{
		m_deltaTime = m_deltaClock.restart().asSeconds();

		if (m_currentState) {
			m_currentState->handleEvents(*this);
			m_currentState->update(m_deltaTime, *this);

			m_window.clear();
			m_currentState->draw(*this);
			m_window.display();
		}
	}
}

// Transition between game states
void GameController::changeGameState(std::unique_ptr<GameState> newState) 
{
	if (newState) 
	{
		m_currentState = std::move(newState);
	}
}

void GameController::loadLevel(int levelNum)
{
	if (levelNum == 1)
	{
		m_score = 0;
		m_kirby->setHealth(6);
		m_kirby->setLife(5);

	}
	if (levelNum >= 2)
	{
		m_score += 100;
	}

	std::string musicFilename = "Level" + std::to_string(levelNum) + ".ogg";

	m_musicManager.play(musicFilename);

	// Load world map and game objects for the specified level
	m_level = std::make_unique<Level>(levelNum, m_kirby.get());
	m_worldMap = m_level->getWorldMap();
	m_fixedObjects = m_level->getObjects();
	m_enemies = m_level->getEnemies();

	spawnKirby();

	// Set the view to the size of the world map
	if (m_worldMap)
	{
		// Calculate the new height for each of the three vertical sections
		float newViewHeight = m_worldMap->getSize().y / 3.0f;

		// Set the game view to the new height
		m_levelAreaHeight = newViewHeight;
		m_gameView.setSize(VIEW_WIDTH, newViewHeight);
	}
}

// Check for collisions between game objects and handle them accordingly
void GameController::checkCollisions()
{
	m_kirby->setInWater(false);
	m_kirby->setGrounded(false);

	// Check for collisions between Kirby and fixed objects
	for (const auto& otherObject : m_fixedObjects)
	{
		if (m_kirby->collidesWith(*otherObject))
		{
			// If Kirby collides with the exit object, mark the level as completed.
			if (otherObject->getType() == ObjectType::EXIT)
			{   
				m_level->setCompleted(true);
				return;
			}

			// For all other objects, let them handle the collision.
			m_kirby->handleCollision(otherObject.get());
		}
	}


	// Check for collisions between Kirby and enemies
	for (const auto& enemy : m_enemies)
	{
		if (m_kirby->collidesWith(*enemy))
		{
			m_kirby->handleCollision(enemy.get());
		}
	}

	// Check for collisions between enemies and other objects
	for (const auto& enemy : m_enemies)
	{
		for (const auto& otherObject : m_fixedObjects) {
			if (enemy->collidesWith(*otherObject))
			{
				enemy->handleCollision(otherObject.get());
				if (otherObject.get()->getType() == ObjectType::WALL)
					break;
			}
		}
	}
}

// Perform a ground check to see if Kirby is on the ground
void GameController::performGroundCheck()
{
	m_kirby->setGrounded(false);

	// Get the bounds of Kirby's ground sensor
	// The ground sensor is a rectangle that extends below Kirby's feet.
	// It is used to check if Kirby is touching the ground.
	sf::FloatRect groundSensorBounds = m_kirby->getGroundSensorBounds();

	// Loop through all fixed objects to find floors
	for (const auto& object : m_fixedObjects)
	{
		if (object->getType() == ObjectType::FLOOR)
		{
			// If the sensor intersects with a floor, set Kirby as grounded.
			if (object->getBounds().intersects(groundSensorBounds))
			{
				m_kirby->setGrounded(true);
				return;
			}
		}
	}
}

// Spawn Kirby at the initial position and set its state to falling
void GameController::spawnKirby()
{
	m_kirby->setPosition(sf::Vector2f(50, 50));
	m_kirby->setState(std::make_unique<KirbyFallingState>());
}

// Update the view to follow Kirby's position
void GameController::updateView()
{
	// Ensure the view is centered on Kirby's position
	float viewX = m_kirby->getPosition().x;
	int verticalBlockIndex = static_cast<int>(m_kirby->getPosition().y / m_levelAreaHeight);
	float viewY = (verticalBlockIndex * m_levelAreaHeight) + (m_levelAreaHeight / 2.f); 

	// Ensure the view does not go out of bounds of the world map.
	sf::FloatRect worldBounds = m_worldMap->getBounds();
	float viewHalfWidth = m_gameView.getSize().x / 2.f;
	float viewHalfHeight = m_gameView.getSize().y / 2.f;

	// Clamp the view position to ensure it stays within the world bounds
	// If the view is too far left or right, adjust it to stay within the bounds.
	if (viewX < viewHalfWidth) { 
		viewX = viewHalfWidth;
	}
	if (viewX > worldBounds.width - viewHalfWidth) {
		viewX = worldBounds.width - viewHalfWidth;
	}
	// If the view is too far up or down adjust it accordingly
	if (viewY < viewHalfHeight) {
		viewY = viewHalfHeight;
	}
	if (viewY > worldBounds.height - viewHalfHeight) {
		viewY = worldBounds.height - viewHalfHeight;
	}
	m_gameView.setCenter(viewX, viewY);
}

// This function is called every frame to update the game state.
// It checks for collisions, updates the positions and states of all game objects,
// and handles the game logic such as checking if Kirby has lost lives.
// It also updates the camera view to follow Kirby's position.
void GameController::update(float deltaTime)
{
	m_kirby->setInWater(false);
	m_kirby->setGrounded(false);

	if (m_kirby->getLives() == 0 && m_kirby->getHealth() == 0)
	{
		m_score = 0;
		m_kirby->setHealth(6);
		m_kirby->setLife(5);
		m_currentState = std::make_unique<MainMenuState>(*this); // Transition to the main menu state

	}

	checkCollisions();
	performGroundCheck();

	m_kirby->update(deltaTime);
	m_kirby->move(deltaTime, m_fixedObjects);

	for (auto& obj : m_fixedObjects)
	{ 
		obj->update(deltaTime); 
	}
	for (auto& enemy : m_enemies)
	{
		enemy->update(deltaTime);
		if (enemy->isSwallowed())
			addScore(enemy->getScoreValue());
	}

	updateView();

	// remove any objects that were marked for deletion during the update phase
	auto it = std::remove_if(m_fixedObjects.begin(), m_fixedObjects.end(),
		[](const std::unique_ptr<GameObject>& obj)
		{
			if (PowerUp* powerUp = dynamic_cast<PowerUp*>(obj.get()))
			{
				return powerUp->isCollected();
			}
			return false;
		});
		m_fixedObjects.erase(it, m_fixedObjects.end());

	// remove swallowed enemies
	auto enemyIterator = std::remove_if(m_enemies.begin(), m_enemies.end(),
		[](const std::unique_ptr<Enemy>& enemy) {
			return enemy->isSwallowed();
		}
	);
	m_enemies.erase(enemyIterator, m_enemies.end());

}

// Handle user input and game events.
// Processes window and keyboard events
void GameController::handleEvents()
{
	// Process window events
	processWindowEvents();

	// Handle Game controll Input
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
		m_window.close(); // Close game window

	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
		m_kirby->setPosition(sf::Vector2f(50, 50)); // Reset Kirby's position

	// Handle Kirby's input
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
	{
		float attackRange = 100.0f; // Define the attack range
		m_kirby->attack(m_enemies, attackRange); // Attack enemies within range
	}
}

// Handle window close, resizing, etc.
void GameController::processWindowEvents()
{
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			m_window.close();
		}
	}
}

// Draw the game objects to the window
void GameController::draw()
{
	m_window.clear(sf::Color::Black);
	m_window.setView(m_gameView);

	m_worldMap->draw(m_window);
	m_kirby->draw(m_window);

	for (const auto& obj : m_fixedObjects)
	{
		obj->draw(m_window);
	}

	for (const auto& enemy : m_enemies)
	{
		enemy->draw(m_window);
	}

	drawHUD();
}

// Initializes the HUD and sets up the game and HUD views.
void GameController::loadHUD()
{
	// Game view - takes up the top portion of the screen
	m_gameView.setSize(VIEW_WIDTH, VIEW_HEIGHT);
	m_gameView.setViewport(sf::FloatRect(0.f, 0.f, 1.f, float(GAME_HEIGHT) / SCREEN_HEIGHT));
	m_levelAreaHeight = VIEW_HEIGHT; // The height of a level section is one view height
	m_gameView.setCenter(m_gameView.getSize().x / 2.f, m_gameView.getSize().y / 2.f);

	// HUD view - takes up the bottom portion of the screen
	m_hudView.setSize(SCREEN_WIDTH, HUD_HEIGHT);
	m_hudView.setViewport(sf::FloatRect(0.f, (float)GAME_HEIGHT / SCREEN_HEIGHT, 1.f, (float)HUD_HEIGHT / SCREEN_HEIGHT)); // ?
	m_hudView.setCenter(SCREEN_WIDTH / 2.f, HUD_HEIGHT / 2.f);

	// Initialize HUD
	m_hud = std::make_unique<HUD>();
	if (!m_hud->loadTexture("HUD.png")) {
		std::cerr << "Warning: Could not load HUD.png\n";
	}

	if (!m_hud->loadSpriteSheet("HUDSpriteSheet.png")) {
		std::cerr << "Warning: Could not load spritesheet.png\n";
	}
}

// Draw the HUD at the bottom of the screen
void GameController::drawHUD()
{
	// Set the HUD view for the bottom section
	m_window.setView(m_hudView);
	
	// Update HUD with current game data
	int kirbyHealth = m_kirby->getHealth();
	int lives = m_kirby->getLives();
	int score = getScore();

	PowerUpType powerUp = m_kirby->getCurrentPower();

	std::string kirbyState;
	if (m_kirby->isInvincible())
		kirbyState = "invincible";
	else if (powerUp == PowerUpType::Spark)
		kirbyState = "spark";
	else if (m_kirby->isHyper())
		kirbyState = "hyper"; 
	else
		kirbyState = "normal";
	
	m_hud->updateGameData(kirbyHealth, lives, score, kirbyState);

	// Draw the HUD stretched across the bottom
	m_hud->draw(m_window);
}

void GameController::addScore(unsigned int points)
{
	m_score += points;
}

unsigned int GameController::getScore() const
{
	return m_score;
}

sf::RenderWindow& GameController::getWindow() 
{
	return m_window;
}

MusicManager& GameController::getMusicManager()
{
	return m_musicManager;
}

Level* GameController::getLevel()
{
	return m_level.get();
}