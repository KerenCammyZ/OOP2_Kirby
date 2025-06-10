#pragma once
#include <SFML/Graphics.hpp>
// Disable MSVC analysis warnings for the box2d include
#pragma warning(push)
#pragma warning(disable: 26495 26813)
#include "box2d/box2d.h"
#pragma warning(pop)

#include "Camera.h"
#include "Renderer.h"
#include "Kirby.h"
#include "WorldMap.h"


class GameController
{
public:
	GameController();
	~GameController() = default;
	void run();
private:
	void update(float deltaTime);
	void handle();
	void render(Renderer& renderer);

	sf::RenderWindow m_window;
	sf::Clock m_deltaClock;
	float m_deltaTime;
	Camera m_camera;
	Renderer m_renderer;
	b2World m_world;

	std::unique_ptr<WorldMap> m_worldMap;
	std::shared_ptr<sf::Texture> m_kirbyTexture;
	std::unique_ptr<Kirby> m_kirby;

};