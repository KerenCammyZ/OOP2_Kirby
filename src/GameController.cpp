#include "GameController.h"
#include "MovingObject.h"

const float zoomFactor = 50.0f;

GameController::GameController():
	m_window(sf::VideoMode(1600, 1200), "Kirby"), m_renderer(m_window), m_world(b2Vec2(0.0f, 9.8f)),
	m_camera(sf::Vector2f(1600.f / zoomFactor, 1200.f / zoomFactor),
		sf::Vector2f(1600.f / zoomFactor, 1200.f / zoomFactor))
{
	m_kirbyTexture = std::make_shared<sf::Texture>();
	if (!m_kirbyTexture->loadFromFile("TestSprite.png"))
	{
		throw std::runtime_error("Failed to load Kirby texture");
	}

	m_kirby = std::make_unique<Kirby>();
	m_kirby->setTexture(m_kirbyTexture);
	
	// Set Kirby's VISUAL size in PIXELS
	sf::Vector2f kirbySize(10.0f, 10.0f);
	m_kirby->setSize(kirbySize);

	// Set Kirby's starting position in PIXELS
	sf::Vector2f startPosition(0,0);

	// Initialize the physics body using pixel coordinates.
	// The function will handle the conversion to meters.
	m_kirby->initPhysics(m_world, startPosition);

	m_worldMapTexture = std::make_shared<sf::Texture>();
	if (!m_worldMapTexture->loadFromFile("Level1.png"))
	{
		throw std::runtime_error("Failed to load world map texture");
	}
	m_worldMap = std::make_unique<WorldMap>();
	m_worldMap->init(m_worldMapTexture);

	sf::Vector2u windowSize = m_window.getSize();
	m_camera.setSize(static_cast<float>(windowSize.x), static_cast<float>(windowSize.y));
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
		m_window.setView(m_camera.getView());
		update(m_deltaTime);
		//draw
		//render(m_renderer);
		m_worldMap->draw(m_window);
		m_kirby->draw(m_window);
		m_window.display();
	}
}

void GameController::update(float deltaTime)
{
	m_world.Step(deltaTime, 8, 3);
	m_kirby->update(deltaTime);

	// Make the camera follow Kirby
	m_camera.setCenter(m_kirby->getPosition());

	//// --- Optional: Clamp camera to map boundaries ---
	//// This prevents the camera from showing empty space outside the map.
	//// You may need to implement this logic in your Camera class.
	//sf::Vector2f mapSize = m_worldMap->getSize();
	//m_camera.clampToArea(sf::FloatRect(0, 0, mapSize.x, mapSize.y));

}

void GameController::handle()
{
}

void GameController::render(Renderer& renderer)
{
	//m_worldMap->draw(m_window);
	//m_kirby->draw(m_window);
}
