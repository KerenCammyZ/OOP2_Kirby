// Camera.h : Manages the visible area of the game world
#pragma once

#include <SFML/Graphics.hpp>

class Camera
{
public:
	Camera(float zoomLevel = 5.0f);
	sf::View getView(sf::Vector2u windowSize);
private:
	float m_zoomLevel;
};