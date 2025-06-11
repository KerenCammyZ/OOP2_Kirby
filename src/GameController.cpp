#include "GameController.h"
#include "MovingObject.h"

GameController::GameController():
	m_window(sf::VideoMode(1600, 1200), "Kirby")
{
	m_kirbyTexture = std::make_shared<sf::Texture>();
	if (!m_kirbyTexture->loadFromFile("TestSprite.png"))
	{
		throw std::runtime_error("Failed to load Kirby texture");
	}

	m_kirby = std::make_unique<Kirby>();
	m_kirby->setTexture(m_kirbyTexture);
	
	// Set Kirby's VISUAL size in PIXELS
	sf::Vector2f kirbySize(50.0f, 50.0f);
	m_kirby->setSize(kirbySize);

	// Set Kirby's starting position in PIXELS
	sf::Vector2f startPosition(0,0);

	m_worldMapTexture = std::make_shared<sf::Texture>();
	if (!m_worldMapTexture->loadFromFile("Level1.png"))
	{
		throw std::runtime_error("Failed to load world map texture");
	}
	m_worldMap = std::make_unique<WorldMap>();
	m_worldMap->init(m_worldMapTexture);
	m_worldMap->setSize(sf::Vector2f(1600.0f, 1200.0f));

	// --- Floor Setup --- (new)
	auto floor = std::make_unique<Floor>();
	floor->setPosition({ 400.f, 500.f });
	floor->setSize({ 300.f, 20.f });      // Make it a wide platform
	m_staticObjects.push_back(std::move(floor));

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
		checkCollisions(); // Check for collisions each frame
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
