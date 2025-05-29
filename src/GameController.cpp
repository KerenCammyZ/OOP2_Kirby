#include "GameController.h"

GameController::GameController():
	m_window(sf::VideoMode(800, 600), "Kirby")
{
	
}

void GameController::run()
{

	while (m_window.isOpen())
	{
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
		update();
		//draw
		draw();
		m_window.display();
	}
}

void GameController::update()
{
}

void GameController::handle()
{
}

void GameController::draw()
{
}
