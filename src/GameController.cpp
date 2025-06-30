// GameController.cpp
#include "GameController.h"
#include <iostream> // for debugging

GameController::GameController():
	m_window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Kirby"), m_currentLevel(1)
{
	// camera setup -- NOTE: camera setup is now handled by loadHUD()
	
	//m_view.setSize(VIEW_WIDTH, VIEW_HEIGHT);
	//m_levelAreaHeight = VIEW_HEIGHT; // The height of a level section is one view height
	//m_view.setCenter(m_view.getSize().x / 2.f, m_view.getSize().y / 2.f);
	
	loadTextures(); // Load Textures of Kirby and Visual World
	loadHUD(); // Load the HUD and set up the views for the game and HUD
	
	//loadCollisionMap("Level1Collisions.png"); // Load the collision map for fixed objects and enemies

}


void GameController::run()
{
	// This is the main outer loop that controls the entire game session.
	while (m_currentLevel <= m_maxLevels)
	{
		// --- Load all assets for the current level ---
		loadLevel(m_currentLevel);

		// This is the inner loop that runs the current level until it's complete.
		while (m_window.isOpen() && !m_level->getCompleted())
		{
			m_deltaTime = m_deltaClock.restart().asSeconds();
			sf::Event event;
			while (m_window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
				{
					// If the window is closed, exit the game entirely.
					m_currentLevel = m_maxLevels + 1; // Set level to exit outer loop. Q: unnecessary? (window is being closed anyway).
					m_window.close();
				}
			}

			if (!m_window.isOpen()) break;

			handle();
			update(m_deltaTime);
			draw();
			m_window.display();
		}

		// If the level was completed (and the window wasn't closed), advance to the next level.
		if (m_window.isOpen()) // Q: "if" is unncessary? (window is necessarily open if we are here)
		{
			m_currentLevel++;
		}
	}
	// Optional: Add a "You Win!" screen here after the loop finishes.
	std::cout << "Game Over! Thanks for playing!" << std::endl;
}

void GameController::checkCollisions()
{
	// --- THIS IS THE FIX ---
	// At the start of the collision phase, assume Kirby is not in water and not on the ground.
	m_kirby->setInWater(false);
	m_kirby->setGrounded(false);

	// Loop through all objects to check for interactions with Kirby.
	for (const auto& otherObject : m_allGameObjects)
	{
		if (m_kirby->collidesWith(*otherObject))
		{
			// The double-dispatch call will handle everything.
			// - If 'otherObject' is a Water tile, its handleCollision will call kirby->setInWater(true).
			// - If 'otherObject' is a Floor tile, its handleCollision will call kirby->setGrounded(true).
			// - If 'otherObject' is an Exit, we handle it specially.

			if (otherObject->getType() == ObjectType::EXIT)
			{
				m_level->setCompleted(true);
				return; // Exit early, a level transition is happening.
			}

			// For all other objects, let them handle the collision.
			m_kirby->handleCollision(otherObject.get());
		}
	}

	// The rest of the collision checks (Kirby vs. Enemies, etc.) remain the same.
	for (const auto& enemy : m_enemies)
	{
		if (m_kirby->collidesWith(*enemy))
		{
			m_kirby->handleCollision(enemy.get());
		}
	}

	for (const auto& enemy : m_enemies)
	{
		for (const auto& otherObject : m_allGameObjects) {
			if (otherObject->getType() == ObjectType::WALL &&
				enemy->collidesWith(*otherObject))
			{
				enemy->handleCollision(static_cast<Wall*>(otherObject.get()));
			}
		}
	}
}


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


// Load Kirby and Worldmap Textures
void GameController::loadTextures()
{
	m_kirbyTexture = std::make_shared<sf::Texture>();
	//m_worldMapTexture = std::make_shared<sf::Texture>();
	
	if (!m_kirbyTexture->loadFromFile("TestSprite.png"))
	{
		throw std::runtime_error("Failed to load Kirby texture");
	}

	/*if (!m_worldMapTexture->loadFromFile("Level1.png"))
	{
		throw std::runtime_error("Failed to load world map texture");
	}*/

	m_kirby = std::make_unique<Kirby>(m_kirbyTexture);
	//m_worldMap = std::make_unique<WorldMap>(m_worldMapTexture);
}


void GameController::loadLevel(int levelNum)
{
	m_kirby->setPosition(sf::Vector2f(50, 50)); // Reset Kirby's position at the start of each level
	m_level = std::make_unique<Level>(levelNum);
	m_worldMap = m_level->getWorldMap(); // Load the world map for the level
	m_allGameObjects = m_level->getObjects(); // Load all objects from the level
	m_enemies = m_level->getEnemies(); // Load all enemies from the level

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


// Update all game objects, including Kirby and enemies
void GameController::update(float deltaTime)
{
	// --- THIS IS THE CORRECT GAME LOOP ORDER ---

	// 1. CHECK THE ENVIRONMENT
	// Run collision checks based on the objects' positions from the last frame.
	// This will set all environmental flags (isGrounded, isInWater, etc.)
	checkCollisions();

	// 2. ACT ON THE ENVIRONMENT
	// Now that all flags are correctly set for this frame, update all objects.
	// Kirby's state machine will now see the correct values for m_isGrounded and m_isInWater.
	m_kirby->update(deltaTime);

	for (auto& obj : m_allGameObjects)
	{
		obj->update(deltaTime);
	}
	for (auto& enemy : m_enemies)
	{
		enemy->update(deltaTime);
	}

	// 3. UPDATE THE CAMERA
	// The view follows the newly calculated positions.
	updateView();

	// 4. CLEAN UP
	// Remove any objects that were marked for deletion during the update phase.
	auto it = std::remove_if(m_allGameObjects.begin(), m_allGameObjects.end(),
		[](const std::unique_ptr<GameObject>& obj)
		{
			if (Present* present = dynamic_cast<Present*>(obj.get()))
			{
				return present->isCollected();
			}
			return false;
		});
	m_allGameObjects.erase(it, m_allGameObjects.end());

	// Enemy removal logic would go here.
}


void GameController::handle()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
		m_kirby->setPosition(sf::Vector2f(50, 50)); // Reset Kirby's position
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
		m_window.close();
}


void GameController::draw()
{
	m_window.clear(sf::Color::Black);
	m_window.setView(m_gameView);

	m_worldMap->draw(m_window);
	m_kirby->draw(m_window);

	// Draw all other objects from our unified list
	for (const auto& obj : m_allGameObjects)
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
	int score = 0;    // TODO: Replace with getScore()
	
	std::string state;
	//if (m_kirby->isInvincible())
		//state = "invincible"; // TODO: Replace with a m_kirby->get() function
	if (m_kirby->isHyper())
		state = "hyper";      // maybe getCurrentPower() / getCurrentState() / getStateDisplay(), ..
	else
		state = "normal";

	m_hud->updateGameData(kirbyHealth, lives, score, state);

	// Draw the HUD stretched across the bottom
	m_hud->draw(m_window);
}