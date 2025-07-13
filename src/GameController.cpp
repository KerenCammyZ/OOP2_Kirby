// GameController.cpp
#include "GameController.h"
#include "ResourceManager.h"
#include "States/GameStates/MainMenuState.h"
#include "States/GameStates/PlayingState.h"
#include "States/KirbyStates/KirbyFallingState.h"
#include <iostream> // for debugging

GameController::GameController():
	m_window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Kirby"), m_currentLevel(1), m_score(0)
{
	// Set the initial state to the Main Menu
	m_currentState = std::make_unique<MainMenuState>(*this);
	m_kirby = std::make_unique<Kirby>(ResourceManager::getTexture("TestSprite.png"));
	loadHUD(); // Load the HUD and set up the views for the game and HUD
}

// Transition between game states
void GameController::changeGameState(std::unique_ptr<GameState> newState) 
{
	if (newState) 
	{
		m_currentState = std::move(newState);
	}
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
		// else throw exception
	}
}


void GameController::loadLevel(int levelNum)
{
	std::string musicFilename = "Level" + std::to_string(levelNum) + ".ogg";

	m_musicManager.play(musicFilename);
	m_level = std::make_unique<Level>(levelNum, m_kirby.get());
	m_worldMap = m_level->getWorldMap(); // Load the world map for the level
	m_fixedObjects = m_level->getObjects(); // Load all objects from the level
	m_enemies = m_level->getEnemies(); // Load all enemies from the level

	spawnKirby(); // Spawn Kirby at the starting position

	// --- CAMERA SETUP PER LEVEL ---
	if (m_worldMap)
	{
		// 1. Calculate the new height for each of the three vertical sections.
		float newViewHeight = m_worldMap->getSize().y / 3.0f;

		// 2. Update our logical variable for camera snapping.
		m_levelAreaHeight = newViewHeight;

		// 3. IMPORTANT: Update the actual sf::View object with the new size.
		// We keep the original width to maintain the aspect ratio you've defined.
		m_gameView.setSize(VIEW_WIDTH, newViewHeight);
	}
}

void GameController::checkCollisions()
{
	// At the start of the collision phase, assume Kirby is not in water and not on the ground.
	m_kirby->setInWater(false);
	m_kirby->setGrounded(false);

	// Loop through all objects to check for interactions with Kirby.
	for (const auto& otherObject : m_fixedObjects)
	{
		if (m_kirby->collidesWith(*otherObject))
		{
			if (otherObject->getType() == ObjectType::EXIT)
			{
				m_level->setCompleted(true);
				return; // Exit early, a level transition is happening.
			}

			// For all other objects, let them handle the collision.
			m_kirby->handleCollision(otherObject.get());
			
			// The double-dispatch call will handle everything.
			// - If 'otherObject' is a Water tile, its handleCollision will call kirby->setInWater(true).
			// - If 'otherObject' is a Floor tile, its handleCollision will call kirby->setGrounded(true).
			// - If 'otherObject' is an Exit, we handle it specially.
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
void GameController::spawnKirby()
{
	m_kirby->setPosition(sf::Vector2f(50, 50)); // Reset Kirby's position
	m_kirby->setState(std::make_unique<KirbyFallingState>());
}

// Update the view to follow Kirby's position
void GameController::updateView()
{
	// horizontal movement
	float viewX = m_kirby->getPosition().x;

	// vertical movement
	//The floor division gives us an integer index (0 for the top block, 1 for the middle, etc.)
	int verticalBlockIndex = static_cast<int>(m_kirby->getPosition().y / m_levelAreaHeight);
	//Calculate the Y-center of that block to lock the camera to it.
	float viewY = (verticalBlockIndex * m_levelAreaHeight) + (m_levelAreaHeight / 2.f);


	// boundry checks
	sf::FloatRect worldBounds = m_worldMap->getBounds();
	float viewHalfWidth = m_gameView.getSize().x / 2.f;
	float viewHalfHeight = m_gameView.getSize().y / 2.f;

	// Left boundary
	if (viewX < viewHalfWidth) {
		viewX = viewHalfWidth;
	}
	// Right boundary
	if (viewX > worldBounds.width - viewHalfWidth) {
		viewX = worldBounds.width - viewHalfWidth;
	}
	// Top boundary
	if (viewY < viewHalfHeight) {
		viewY = viewHalfHeight;
	}
	// Bottom boundary
	if (viewY > worldBounds.height - viewHalfHeight) {
		viewY = worldBounds.height - viewHalfHeight;
	}

	// Set the view's final center position for this frame.
	m_gameView.setCenter(viewX, viewY);
}

// This function is called every frame to update the game state.
// It checks for collisions, updates the positions and states of all game objects,
// and handles the game logic such as checking if Kirby has lost lives.
// It also updates the camera view to follow Kirby's position.
void GameController::update(float deltaTime)
{
	if (m_kirby->getLives() == 0 && m_kirby->getHealth() == 0)
	{
		// If Kirby has no health or lives left, end the game.
		m_window.close();
		std::cout << "Game Over! Kirby has no health or lives left." << std::endl;
		exit(EXIT_SUCCESS);
	}

	// 1. CHECK THE ENVIRONMENT
	// Run collision checks based on the objects' positions from the last frame.
	// This will set all environmental flags (isGrounded, isInWater, etc.)
	checkCollisions();

	// 2. ACT ON THE ENVIRONMENT
	// Now that all flags are correctly set for this frame, update all objects.
	// Kirby's state machine will now see the correct values for m_isGrounded and m_isInWater.
	m_kirby->update(deltaTime);
	m_kirby->move(deltaTime, m_fixedObjects);
	 
	for (auto& obj : m_fixedObjects)
	{
		obj->update(deltaTime);
	}

	for (auto& enemy : m_enemies)
	{
		if(enemy->isSwallowed())
			addScore(enemy->getScoreValue());

		enemy->update(deltaTime);
	}

	m_kirby->setInWater(false);
	m_kirby->setGrounded(false);

	// update the camera
	updateView();


	// 4. CLEAN UP
	// Remove any objects that were marked for deletion during the update phase.
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
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
		m_kirby->setPosition(sf::Vector2f(50, 50)); // Reset Kirby's position

	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
		m_window.close(); // Close game window

	// Handle Kirby's input
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
	{
		float attackRange = 100.0f; // Define the attack range
		m_kirby->attack(m_enemies, attackRange); // Attack enemies within range
	}
	/*if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		m_kirby->moveLeft();
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		m_kirby->moveRight();
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		m_kirby->jump();
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		m_kirby->crouch();
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		m_kirby->attack();
	}*/
}
void GameController::draw()
{
	m_window.clear(sf::Color::Black);
	m_window.setView(m_gameView);

	m_worldMap->draw(m_window);
	m_kirby->draw(m_window);

	// Draw all other objects from our unified list
	for (const auto& obj : m_fixedObjects)
	{
		obj->draw(m_window);
	}

	// Draw all enemies
	for (const auto& enemy : m_enemies)
	{
		enemy->draw(m_window);
	}

	// NOTE: If you were to draw UI elements (like a score or health bar),
	// you would switch back to the default view here so they stay fixed on the screen:
	// m_window.setView(m_window.getDefaultView());
	// ... draw UI ...
	drawHUD();
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
// Load the HUD and set up the views for the game and HUD
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
		std::cout << "Warning: Could not load HUD.png\n";
	}

	if (!m_hud->loadSpriteSheet("HUDSpriteSheet.png")) {
		std::cout << "Warning: Could not load spritesheet.png\n";
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

	std::string kirbyState;
	//if (m_kirby->isInvincible())
		//state = "invincible"; // TODO: Replace with a m_kirby->get() function
	if (m_kirby->isHyper())
		kirbyState = "hyper";      // maybe getCurrentPower() / getCurrentState() / getStateDisplay(), ..
	else
		kirbyState = "normal";

	m_hud->updateGameData(kirbyHealth, lives, score, kirbyState);

	// Draw the HUD stretched across the bottom
	m_hud->draw(m_window);
}

void GameController::addScore(unsigned int points)
{
	unsigned int oldScore = m_score;
	m_score += points;

	// Trigger score animation in HUD when score increases
	//if (m_hud && m_score > oldScore) {
	//	m_hud->triggerScoreAnimation();
	//}
}

unsigned int GameController::getScore() const
{
	return m_score;
}