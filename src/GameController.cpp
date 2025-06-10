#include "GameController.h"
#include "MovingObject.h"

GameController::GameController():
	m_window(sf::VideoMode(1600, 1200), "Kirby"), m_renderer(m_window), m_world(b2Vec2(0.0f, 9.8f))
{
	m_kirbyTexture = std::make_shared<sf::Texture>();
	if (!m_kirbyTexture->loadFromFile("TestSprite.png"))
	{
		throw std::runtime_error("Failed to load Kirby texture");
	}

	m_kirby = std::make_unique<Kirby>();
	m_kirby->setTexture(m_kirbyTexture);
	m_kirby->initPhysics(m_world, b2Vec2(0.0f, 0.0f), b2Vec2(0.0f, 0.0f));

	m_worldMapTexture = std::make_shared<sf::Texture>();
	if(!m_worldMapTexture->loadFromFile("Level1Map.png"))
	{
		throw std::runtime_error("Failed to load world map texture");
	}
	m_worldMap = std::make_unique<WorldMap>();
	m_worldMap->init(m_worldMapTexture);
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
		m_window.setView(m_camera.getView(m_window.getSize()));
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
}

void GameController::handle()
{
}

void GameController::render(Renderer& renderer)
{
	m_worldMap->draw(renderer);
	m_kirby->draw(renderer);
}
