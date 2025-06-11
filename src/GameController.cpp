#include "GameController.h"
#include "MovingObject.h"

GameController::GameController():
	m_window(sf::VideoMode(1600, 1200), "Kirby")
{
	// kirby setup
	m_kirbyTexture = std::make_shared<sf::Texture>();
	if (!m_kirbyTexture->loadFromFile("TestSprite.png"))
	{
		throw std::runtime_error("Failed to load Kirby texture");
	}
	m_kirby = std::make_unique<Kirby>();
	m_kirby->setTexture(m_kirbyTexture);
	sf::Vector2f kirbySize(50.0f, 50.0f);
	m_kirby->setSize(kirbySize);
	sf::Vector2f startPosition(0,0);

	// worldmap setup
	m_worldMapTexture = std::make_shared<sf::Texture>();
	if (!m_worldMapTexture->loadFromFile("Level1.png"))
	{
		throw std::runtime_error("Failed to load world map texture");
	}
	
	// --- NEW: CALCULATE SCALE ---
// 1. Get the original size of the texture
	sf::Vector2f originalMapSize = sf::Vector2f(m_worldMapTexture->getSize());

	// 2. Define your target display size
	sf::Vector2f targetMapSize(1600.0f, 1200.0f); //

	// 3. Calculate the scale factors
	sf::Vector2f mapScale(targetMapSize.x / originalMapSize.x,
		targetMapSize.y / originalMapSize.y);


	// --- Initialize and set the size of the visual map ---
	m_worldMap = std::make_unique<WorldMap>();
	m_worldMap->init(m_worldMapTexture); 
	m_worldMap->setSize(targetMapSize); 

	////floor setup (debugging purposes)
	//auto floor = std::make_unique<Floor>();
	//floor->setPosition({ 400.f, 500.f });
	//floor->setSize({ 300.f, 20.f });      // Make it a wide platform
	//m_staticObjects.push_back(std::move(floor));


	// --- Load Collisions using the scale factor ---
	sf::Image collisionImage;
	if (!collisionImage.loadFromFile("Level1Collisions.png"))
	{
		throw std::runtime_error("Failed to load collision map image");
	}

	// Pass the calculated scale factor to the function
	auto collidablesFromFile = m_worldMap->loadCollisions(collisionImage, mapScale);

	m_staticObjects.insert(
		m_staticObjects.end(),
		std::make_move_iterator(collidablesFromFile.begin()),
		std::make_move_iterator(collidablesFromFile.end()));

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
}

void GameController::update(float deltaTime)
{
	m_kirby->update(deltaTime);
	checkCollisions();
}

void GameController::handle()
{
}

void GameController::draw()
{
	m_worldMap->draw(m_window);
	m_kirby->draw(m_window);
	for (const auto& obj : m_staticObjects)
	{
		obj->draw(m_window);
	}
}
