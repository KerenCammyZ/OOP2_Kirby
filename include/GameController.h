#pragma once
#include <SFML/Graphics.hpp>

#include "Camera.h"
#include "Renderer.h"

class GameController
{
public:
	GameController();
	~GameController() = default;
	void run();
private:
	void update();
	void handle();
	void render(Renderer& renderer);

	sf::RenderWindow m_window;
	sf::Clock m_deltaClock;
	Camera m_camera;
	Renderer m_renderer;

};