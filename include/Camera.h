// Camera.h
#pragma once
#include <SFML/Graphics.hpp>

class Camera {
public:
    Camera(sf::Vector2f initialViewSize);
    void setTarget(sf::Vector2f targetPos);
    void update(float deltaTime);
    void applyTo(sf::RenderWindow& window) const;
    void setLevelBounds(sf::FloatRect bounds);
    void handleWindowResize(unsigned int newWidth, unsigned int newHeight);

    sf::Vector2f getCenter() const { return m_view.getCenter(); }
    sf::FloatRect getViewBounds() const;

private:
    sf::View m_view;
    sf::Vector2f m_targetPosition;
    sf::FloatRect m_currentLevelBounds;
    //float m_smoothing = 5.0f;
};
