#pragma once
#include <SFML/Graphics.hpp>

class GameController
{
public:
	GameController();
	~GameController() = default;
	void run();
private:
	void update();
	void handle();
	void draw();

	sf::RenderWindow m_window;

};