#include "GameController.h"
#include "MovingObject.h"

GameController::GameController():
	m_window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Kirby")
{
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
	
	sf::Vector2f originalMapSize = sf::Vector2f(m_worldMapTexture->getSize());
	sf::Vector2f targetMapSize(SCREEN_WIDTH, SCREEN_HEIGHT);

	// Calculate the scale factors
	sf::Vector2f mapScale(targetMapSize.x / originalMapSize.x,
		targetMapSize.y / originalMapSize.y);

	// Initialize and set the size of the visual map
	m_worldMap = std::make_unique<WorldMap>();
	m_worldMap->init(m_worldMapTexture); 
	m_worldMap->setSize(targetMapSize); 


	// Load Collisions using the scale factor
	sf::Image collisionImage;
	if (!collisionImage.loadFromFile("Level1Collisions.png"))
	{
		throw std::runtime_error("Failed to load collision map image");
	}

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
	for (const auto& staticObject : m_staticObjects)
	{
		if (m_kirby->collidesWith(*staticObject))
		{
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
