#include "GameController.h"

GameController::GameController():
	m_window(sf::VideoMode(800, 600), "Kirby"), m_renderer(m_window){}

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

		m_window.clear(sf::Color::Black);
		//update
		m_window.setView(m_camera.getView(m_window.getSize()));
		update();
		//draw
		render(m_renderer);
		m_window.display();
	}
}

void GameController::update()
{
}

void GameController::handle()
{
}

void GameController::render(Renderer& renderer)
{
	
	//test object
	sf::Texture texture;
	if (!texture.loadFromFile("TestSprite.png"))
	{
		throw std::runtime_error("Failed to load texture");
	}
	renderer.draw(texture, sf::Vector2f(), sf::Vector2f(0.5,0.5));
}
