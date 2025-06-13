#include "GameController.h"
#include "MovingObject.h"

GameController::GameController():
	m_window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Kirby")
{
	m_window.setFramerateLimit(60);

	// kirby setup
	m_kirbyTexture = std::make_shared<sf::Texture>();
	if (!m_kirbyTexture->loadFromFile("TestSprite.png"))
	{
		throw std::runtime_error("Failed to load Kirby texture");
	}
	m_kirby = std::make_unique<Kirby>();
	m_kirby->setTexture(m_kirbyTexture);
	sf::Vector2f kirbySize(ENTITY_SIZE, ENTITY_SIZE);
	m_kirby->setSize(kirbySize);
	sf::Vector2f startPosition(0,0);

	// worldmap setup
	m_worldMapTexture = std::make_shared<sf::Texture>();
	if (!m_worldMapTexture->loadFromFile("Level1.png"))
	{
		throw std::runtime_error("Failed to load world map texture");
	}

	// Get the original size of the texture (this defines your total world dimensions)
	sf::Vector2f originalMapSize = sf::Vector2f(m_worldMapTexture->getSize());
	// Initialize and set the size of the visual map to its original texture dimensions
	// This means 1 pixel in Level1.png = 1 world unit.
	m_worldMap = std::make_unique<WorldMap>();
	m_worldMap->init(m_worldMapTexture);
	m_worldMap->setSize(originalMapSize);

	//// --- CALCULATE SCALE ---
	//// 1. Get the original size of the texture
	//sf::Vector2f originalMapSize = sf::Vector2f(m_worldMapTexture->getSize());
	//// 2. Define your target display size
	//sf::Vector2f targetMapSize(SCREEN_WIDTH, SCREEN_HEIGHT); //
	//// 3. Calculate the scale factors
	//sf::Vector2f mapScale(targetMapSize.x / originalMapSize.x, targetMapSize.y / originalMapSize.y);

	//// --- Initialize and set the size of the visual map ---
	//m_worldMap = std::make_unique<WorldMap>();
	//m_worldMap->init(m_worldMapTexture); 
	//m_worldMap->setSize(targetMapSize);

	//// --- Load Collisions using the scale factor ---
	sf::Image collisionImage;
	if (!collisionImage.loadFromFile("Level1Collisions.png"))
	{
		throw std::runtime_error("Failed to load collision map image");
	}
	// For collisions, assume 1 pixel in collision map = 1 world unit.
	// The WorldMap::loadCollisions function should handle scaling if needed,
	// but mapScale should be (1.f, 1.f) if collision map is in world units.
	sf::Vector2f collisionMapScale(1.f, 1.f); // Assuming collision map is 1:1 with world units
	auto collidablesFromFile = m_worldMap->loadCollisions(collisionImage, collisionMapScale);

	m_staticObjects.insert(
		m_staticObjects.end(),
		std::make_move_iterator(collidablesFromFile.begin()),
		std::make_move_iterator(collidablesFromFile.end()));

	// --- Camera Setup ---
	// Define the desired height of the view in world units (e.g., 200 units tall)
	float desiredViewHeight = float(SECTION_HEIGHT); // Adjust this to control "zoom"
	float initialAspect = (float)m_window.getSize().x / (float)m_window.getSize().y;
	sf::Vector2f initialViewSize(desiredViewHeight * initialAspect, desiredViewHeight);

	m_camera = std::make_unique<Camera>(initialViewSize); // Initialize camera with desired view size
	m_camera->setTarget(m_kirby->getPosition()); // Set initial camera target to Kirby


	m_levelManager = std::make_unique<LevelManager>();
	m_levelManager->loadLevel("Level1.png");// Pass the full world texture


	// Set initial camera bounds based on the first level section's world bounds
	LevelSection& firstSection = m_levelManager->getCurrentSectionData();
	m_camera->setLevelBounds(firstSection.worldBounds);

	////floor setup (debugging purposes)
	//auto floor = std::make_unique<Floor>();
	//floor->setPosition({ 400.f, 500.f });
	//floor->setSize({ 300.f, 20.f });      // Make it a wide platform
	//m_staticObjects.push_back(std::move(floor));
}

void GameController::run()
{	
	while (m_window.isOpen())
	{
		m_deltaTime = m_deltaClock.restart().asSeconds();
		sf::Event event;
		while (m_window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				m_window.close();

			// Handle level transition when 'N' is pressed
			else if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Escape)
					m_window.close();
				
				else if (event.key.code == sf::Keyboard::N)
					handleLevelTransition();
			}
			// Handle window resizing
			else if (event.type == sf::Event::Resized)
			{
				m_camera->handleWindowResize(event.size.width, event.size.height);
			}
		}

		//handle
		handle();

		//update
		m_window.clear(sf::Color::Black);
		update(m_deltaTime);
		draw();
		m_window.display();
	}
}

void GameController::checkCollisions()
{
	// Iterate through all the static objects in the game
	for (const auto& staticObject : m_staticObjects)
	{
		// Use the collidesWith function to check for an intersection
		if (m_kirby->collidesWith(*staticObject))
		{
			// If they collide, delegate handling to Kirby.
			// This begins the double-dispatch process.
			m_kirby->handleCollision(staticObject.get());
		}
	}

	// Clamp Kirby's position to current level bounds
	LevelSection& currentSection = m_levelManager->getCurrentSectionData();
    sf::FloatRect levelBounds = currentSection.worldBounds;
    sf::Vector2f kirbyPos = m_kirby->getPosition();
    sf::Vector2f kirbySize = m_kirby->getSize(); // Assuming size is correct for collision/drawing

    // Clamp X position
    if (kirbyPos.x - kirbySize.x / 2.f < levelBounds.left) {
        kirbyPos.x = levelBounds.left + kirbySize.x / 2.f;
    } else if (kirbyPos.x + kirbySize.x / 2.f > levelBounds.left + levelBounds.width) {
        kirbyPos.x = levelBounds.left + levelBounds.width - kirbySize.x / 2.f;
    }

    // Clamp Y position
    if (kirbyPos.y - kirbySize.y / 2.f < levelBounds.top) {
        kirbyPos.y = levelBounds.top + kirbySize.y / 2.f;
    } else if (kirbyPos.y + kirbySize.y / 2.f > levelBounds.top + levelBounds.height) {
        kirbyPos.y = levelBounds.top + levelBounds.height - kirbySize.y / 2.f;
    }
    m_kirby->setPosition(kirbyPos); // Apply the clamped position
}

void GameController::update(float deltaTime)
{
	m_kirby->update(deltaTime);
	checkCollisions();

	sf::Vector2f kirbyPos = m_kirby->getPosition();
	m_camera->setTarget(kirbyPos); // Tell camera to follow Kirby
	m_camera->update(deltaTime); // Update camera's internal view
}

void GameController::handleLevelTransition() {
	
	// Transition to next section
	m_levelManager->transitionToNext();

	// Update camera bounds for new section
	LevelSection& newSection = m_levelManager->getCurrentSectionData();
	m_camera->setLevelBounds(newSection.worldBounds);

	// Reset Kirby position to start of new section
	m_kirby->setPosition(sf::Vector2f(50, newSection.worldBounds.top));
}

void GameController::handle()
{
}

void GameController::draw()
{
	m_camera->applyTo(m_window); // Apply camera view
	m_worldMap->draw(m_window);

	for (const auto& obj : m_staticObjects)// Draw objects (camera view will automatically crop them)
	{
		obj->draw(m_window);
	}
	m_kirby->draw(m_window);

	// Reset to default view for UI elements (if you add them later)
	m_window.setView(m_window.getDefaultView());
	// Draw UI here...
}
