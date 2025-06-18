// GameController.cpp
#include "GameController.h"

GameController::GameController():
	m_window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Kirby"), m_deltaTime(0.f)
{
	// camera setup
	m_view.setSize(VIEW_WIDTH, VIEW_HEIGHT);

	m_view.setCenter(m_view.getSize().x / 2.f, m_view.getSize().y / 2.f);


	loadTextures(); // TODO: move to resource manager
	m_kirby = std::make_unique<Kirby>(m_kirbyTexture);
	m_worldMap = std::make_unique<WorldMap>(m_worldMapTexture);
	
	loadCollisionMap();
	auto collidablesFromFile = m_worldMap->loadCollisions();

	m_FixedObjects.insert(
		m_FixedObjects.end(),
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

		handle();
		update(m_deltaTime);
		draw();
		m_window.display();
	}
}

void GameController::checkCollisions()
{
	for (const auto& FixedObject : m_FixedObjects)
	{
		if (m_kirby->collidesWith(*FixedObject))
		{
			m_kirby->handleCollision(FixedObject.get());
		}
	}
}

void GameController::updateView()
{
	// horizontal movement
	float viewX = m_kirby->getPosition().x;

	// vertical movement
	float viewY = m_view.getSize().y / 2.f;


	// boundaries for the camera view
	sf::FloatRect worldBounds = m_worldMap->getBounds();
	float viewHalfWidth = m_view.getSize().x / 2.f;

	if (viewX < viewHalfWidth)
	{
		viewX = viewHalfWidth;
	}
	if (viewX > worldBounds.width - viewHalfWidth)
	{
		viewX = worldBounds.width - viewHalfWidth;
	}

	// Set the view's final center position for this frame.
	m_view.setCenter(viewX, viewY);
}

// Load kirby and worldmap textures
void GameController::loadTextures()
{
	m_kirbyTexture = std::make_shared<sf::Texture>();
	m_worldMapTexture = std::make_shared<sf::Texture>();
	
	if (!m_kirbyTexture->loadFromFile("TestSprite.png"))
	{
		throw std::runtime_error("Failed to load Kirby texture");
	}

	if (!m_worldMapTexture->loadFromFile("Level1.png"))
	{
		throw std::runtime_error("Failed to load world map texture");

	}
}

// Load the collision map and set up static objects
void GameController::loadCollisionMap()
{
	auto collisionImage = std::make_unique<sf::Image>();
	if (!collisionImage->loadFromFile("Level1Collisions.png"))
	{
		throw std::runtime_error("Failed to load collision map image");
	}
	m_worldMap->setCollisionMap(std::move(collisionImage));
}

void GameController::update(float deltaTime)
{
	m_kirby->update(deltaTime);
	checkCollisions();
	updateView(); // Update the camera's position every frame
}

void GameController::handle()
{
}

void GameController::draw()
{
	m_window.clear(sf::Color::Black);
	m_window.setView(m_view);
	m_worldMap->draw(m_window);
	m_kirby->draw(m_window);

	for (const auto& obj : m_FixedObjects)
	{
		obj->draw(m_window);
	}

	// NOTE: If you were to draw UI elements (like a score or health bar),
	// you would switch back to the default view here so they stay fixed on the screen:
	// m_window.setView(m_window.getDefaultView());
	// ... draw UI ...
}
