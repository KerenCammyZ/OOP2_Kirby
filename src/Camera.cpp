#include "Camera.h"

Camera::Camera(float zoomLevel): m_zoomLevel(zoomLevel)
{
}

sf::View Camera::getView(sf::Vector2u windowSize)
{
	float aspect = (float)windowSize.x / (float)windowSize.y;
	sf::Vector2f size{};
	if (aspect < 1.0f)
		size = sf::Vector2f(m_zoomLevel, m_zoomLevel / aspect);
	else
		size = sf::Vector2f(m_zoomLevel * aspect, m_zoomLevel);

	return sf::View(sf::Vector2f(0, 0), size);
}


