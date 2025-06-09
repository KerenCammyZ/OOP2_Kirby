#include "GameController.h"
#include "MovingObject.h"
#include "StaticObject.h"

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
	m_kirby->initPhysics(m_world, b2Vec2(0.01f, 0.01f), b2Vec2(0.01f, 0.01f));

	// Get the initial view of the camera
	sf::View initialView = m_camera.getView(m_window.getSize());

	// Get the center of the initial view
	sf::Vector2f viewCenter = initialView.getCenter();

	// Position Kirby near the center of the view
	m_kirby->setPosition(viewCenter);
}

void GameController::run()
{	
	//create test object
	auto testObject = std::make_unique<StaticObject>(m_world, b2Vec2(0.0f, 0.0f));
	//draw a test square where the test object is located
	sf::RectangleShape testShape(sf::Vector2f(10.0f * 30.0f, 10.0f * 30.0f));
	testShape.setPosition(testObject->getPosition().x * 30.0f, testObject->getPosition().y * 30.0f);

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
		render(m_renderer);
		m_window.draw(testShape);
		
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
	m_kirby->draw(renderer);
}
