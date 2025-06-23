// GameController.cpp
#include "GameController.h"

GameController::GameController():
	m_window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Kirby"), m_deltaTime(0.f)
{
	// camera setup
	m_view.setSize(VIEW_WIDTH, VIEW_HEIGHT);
	m_levelBlockHeight = VIEW_HEIGHT; // The height of a level section is one view height
	m_view.setCenter(m_view.getSize().x / 2.f, m_view.getSize().y / 2.f);

	loadTextures(); // TODO: move to resource manager
	m_kirby = std::make_unique<Kirby>(m_kirbyTexture);
	m_worldMap = std::make_unique<WorldMap>(m_worldMapTexture);
	
	m_allGameObjects = m_worldMap->loadObjectsFromFile("Level1Collisions.png");
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
	for (const auto& otherObject : m_allGameObjects)
	{
		if (m_kirby->collidesWith(*otherObject))
		{
			// Double dispatch will handle the specific collision type
			m_kirby->handleCollision(otherObject.get());
		}
	}
}

void GameController::updateView()
{
	// horizontal movement
	float viewX = m_kirby->getPosition().x;

	// vertical movement
	//The floor division gives us an integer index (0 for the top block, 1 for the middle, etc.)
	int verticalBlockIndex = static_cast<int>(m_kirby->getPosition().y / m_levelBlockHeight);
	//Calculate the Y-center of that block to lock the camera to it.
	float viewY = (verticalBlockIndex * m_levelBlockHeight) + (m_levelBlockHeight / 2.f);


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

void GameController::update(float deltaTime)
{
	m_kirby->update(deltaTime);
	// Update all other objects loaded from the map
	for (auto& obj : m_allGameObjects)
	{
		// Polymorphism calls the correct update (MovingObject vs FixedObject)
		obj->update(deltaTime);
	}
	checkCollisions();
	updateView(); // Update the camera's position every frame
}

void GameController::handle()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
		m_kirby->setPosition(sf::Vector2f(50, 50)); // Reset Kirby's position
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

	// NOTE: If you were to draw UI elements (like a score or health bar),
	// you would switch back to the default view here so they stay fixed on the screen:
	// m_window.setView(m_window.getDefaultView());
	// ... draw UI ...
}
