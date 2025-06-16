#include "GameController.h"
#include "MovingObject.h"

GameController::GameController():
	m_window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Kirby")
{
	// VIEW (CAMERA) SETUP
	// -------------------------------------------------------------------
	// Set the size of the view. This controls the "zoom" level.
	// A smaller size makes everything appear larger (more zoomed in).
	m_view.setSize(VIEW_WIDTH, VIEW_HEIGHT);

	// Set the initial center of the view. The Y-position will be locked.
	// We center it horizontally and vertically based on its own size,
	// focusing on the top-left part of the world.
	m_view.setCenter(m_view.getSize().x / 2.f, m_view.getSize().y / 2.f);

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
	sf::Vector2f startPosition(50,50);
	m_kirby->setPosition(startPosition);

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

		handle();
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

void GameController::updateView()
{
	// --- HORIZONTAL MOVEMENT ---
	// The view's horizontal center should follow Kirby's position.
	float viewX = m_kirby->getPosition().x;

	// --- VERTICAL MOVEMENT (LOCKED) ---
	// The view's vertical center is fixed. We get the view's current height
	// and divide by two. This ensures the "ground" level is always at the same
	// vertical position on the screen, creating a classic side-scroller feel.
	float viewY = m_view.getSize().y / 2.f;


	// --- BOUNDARY CHECKS ---
	// Prevent the camera from showing areas outside the level map.
	sf::FloatRect worldBounds = m_worldMap->getBounds();
	float viewHalfWidth = m_view.getSize().x / 2.f;

	// Left boundary: Don't let the view go past the left edge of the world.
	if (viewX < viewHalfWidth)
	{
		viewX = viewHalfWidth;
	}
	// Right boundary: Don't let the view go past the right edge of the world.
	if (viewX > worldBounds.width - viewHalfWidth)
	{
		viewX = worldBounds.width - viewHalfWidth;
	}

	// Set the view's final center position for this frame.
	m_view.setCenter(viewX, viewY);
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
	for (const auto& obj : m_staticObjects)
	{
		obj->draw(m_window);
	}

	// NOTE: If you were to draw UI elements (like a score or health bar),
	// you would switch back to the default view here so they stay fixed on the screen:
	// m_window.setView(m_window.getDefaultView());
	// ... draw UI ...
}
