// GameController.cpp
#include "GameController.h"
#include <iostream> // for debugging

GameController::GameController():
	m_window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Kirby"), m_currentLevel(1)
{
	// camera setup
	m_view.setSize(VIEW_WIDTH, VIEW_HEIGHT);
	m_levelAreaHeight = VIEW_HEIGHT; // The height of a level section is one view height
	m_view.setCenter(m_view.getSize().x / 2.f, m_view.getSize().y / 2.f);

	loadTextures(); // Load Textures of Kirby and Visual World
	//loadCollisionMap("Level1Collisions.png"); // Load the collision map for fixed objects and enemies

}

//void GameController::run()
//{	
//	loadLevel(1); // Load the first level
//	while (m_window.isOpen())
//	{
//		m_deltaTime = m_deltaClock.restart().asSeconds();
//		sf::Event event;
//		while (m_window.pollEvent(event))
//		{
//			if (event.type == sf::Event::Closed)
//				m_window.close();
//		}
//
//		handle();
//		update(m_deltaTime);
//		draw();
//		m_window.display();
//	}
//}

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
					m_currentLevel = m_maxLevels + 1; // Set level to exit outer loop
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
		if (m_window.isOpen())
		{
			m_currentLevel++;
		}
	}
	// Optional: Add a "You Win!" screen here after the loop finishes.
}

void GameController::checkCollisions()  
{  
	// Check collisions between Kirby and fixed objects
	for (const auto& otherObject : m_allGameObjects)
	{
		if (m_kirby->collidesWith(*otherObject))
		{
			// Double dispatch will handle the specific collision type  
			m_kirby->handleCollision(otherObject.get());
		}
	}

	// Check collisions between Kirby and enemies
	for (const auto& enemy : m_enemies)
	{
		if (m_kirby->collidesWith(*enemy))
		{
			m_kirby->handleCollision(enemy.get());
		}
	}

	// Check enemy collisions with fixed objects
	for (const auto& enemy : m_enemies)
	{
		for (const auto& otherObject : m_allGameObjects) {
			if (otherObject->getType() == ObjectType::WALL &&
				enemy->collidesWith(*otherObject))
			{
				// Double dispatch will handle the specific collision type  
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
	float viewHalfWidth = m_view.getSize().x / 2.f;
	float viewHalfHeight = m_view.getSize().y / 2.f;

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
	m_view.setCenter(viewX, viewY);
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
	m_level = std::make_unique<Level>(levelNum);
	m_worldMap = m_level->getWorldMap(); // Load the world map for the level
	m_allGameObjects = m_level->getObjects(); // Load all objects from the level
	m_enemies = m_level->getEnemies(); // Load all enemies from the level
}

//// Load the collision map for fixed objects and enemies
//void GameController::loadCollisionMap(std::string collisionMap)
//{
//	// Load all objects from the collision map file
//	auto objects = m_worldMap->loadObjectsFromFile(collisionMap);
//
//	// separate enemies from fixed objects (load enemies)
//	for (auto it = objects.begin(); it != objects.end(); )
//	{   
//		if (auto enemy = dynamic_cast<Enemy*>(it->get()))
//		{
//			m_enemies.push_back(std::unique_ptr<Enemy>(static_cast<Enemy*>(it->release())));
//			it = objects.erase(it);
//		}
//		else {
//			++it;
//		}
//	}
//	m_allGameObjects = std::move(objects); // contains only fixed objects (no enemies)
//	std::cout << "Loaded " << m_enemies.size() << " enemies.\n";
//}

// Update all game objects, including Kirby and enemies
void GameController::update(float deltaTime)
{
	m_kirby->update(deltaTime);
	
	// Update all other objects loaded from the map
	for (auto& obj : m_allGameObjects)
	{
		// Polymorphism calls the correct update (MovingObject vs FixedObject)
		obj->update(deltaTime);
	}

	for (auto& enemy : m_enemies)
	{
		enemy->update(deltaTime);
	}

	checkCollisions();
	updateView(); // Update the camera's position every frame 

	// --- NEW: Remove collected presents ---Add commentMore actions
	// std::remove_if moves all elements to be deleted to the end of the vector
	// and returns an iterator to the first of those elements.
	auto it = std::remove_if(m_allGameObjects.begin(), m_allGameObjects.end(),
		[](const std::unique_ptr<GameObject>& obj)
		{
			// Try to cast the object to a Present*
			if (Present* present = dynamic_cast<Present*>(obj.get()))
			{
				// If it is a present and it's been collected, mark it for removal
				return present->isCollected();
			}
			return false; // Not a present, don't remove
		});

	// .erase() then actually removes the elements from the vector.
	m_allGameObjects.erase(it, m_allGameObjects.end());
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
	m_window.setView(m_view);

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
}
