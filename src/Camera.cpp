// Camera.cpp
#include "Camera.h"
#include <algorithm>

Camera::Camera(sf::Vector2f initialViewSize)
	: m_view(sf::Vector2f(0, 0), initialViewSize),
	  m_targetPosition(0, 0),
	  m_currentLevelBounds(0, 0, 0, 0)

{
}

void Camera::setTarget(sf::Vector2f targetPos)
{
    m_targetPosition = targetPos;
}

void Camera::setLevelBounds(sf::FloatRect bounds) {
    m_currentLevelBounds = bounds;
}

void Camera::update(float deltaTime)
{
    // --- Camera following logic ---
    // For now, directly set center to target.
    // For smoothing, you'd use lerp: m_view.setCenter(m_view.getCenter() + (m_target - m_view.getCenter()) * m_lerpSpeed * deltaTime);
    sf::Vector2f currentCenter = m_view.getCenter();
    sf::Vector2f newCenter = m_targetPosition;

    // --- Clamp camera within level bounds ---
    sf::Vector2f viewHalfSize = m_view.getSize() / 2.f;

    // Calculate effective min/max x and y for the center
    float minX = m_currentLevelBounds.left + viewHalfSize.x;
    float maxX = m_currentLevelBounds.left + m_currentLevelBounds.width - viewHalfSize.x;
    float minY = m_currentLevelBounds.top + viewHalfSize.y;
    float maxY = m_currentLevelBounds.top + m_currentLevelBounds.height - viewHalfSize.y;

    // Clamp the new center
    if (newCenter.x < minX) newCenter.x = minX;
    if (newCenter.x > maxX) newCenter.x = maxX;
    if (newCenter.y < minY) newCenter.y = minY;
    if (newCenter.y > maxY) newCenter.y = maxY;

    // Handle cases where level bounds are smaller than the view
    if (m_currentLevelBounds.width < m_view.getSize().x) {
        newCenter.x = m_currentLevelBounds.left + m_currentLevelBounds.width / 2.f;
    }
    if (m_currentLevelBounds.height < m_view.getSize().y) {
        newCenter.y = m_currentLevelBounds.top + m_currentLevelBounds.height / 2.f;
    }

    m_view.setCenter(newCenter);
}

void Camera::applyTo(sf::RenderWindow& window) const
{
    window.setView(m_view);
}

sf::FloatRect Camera::getViewBounds() const {
    sf::Vector2f center = m_view.getCenter();
    sf::Vector2f size = m_view.getSize();
    return sf::FloatRect(
        center.x - size.x / 2.f,
        center.y - size.y / 2.f,
        size.x,
        size.y
    );
}


void Camera::handleWindowResize(unsigned int newWidth, unsigned int newHeight) {
    // Maintain the vertical field of view, adjust width based on new aspect ratio
    float newAspect = (float)newWidth / (float)newHeight;
    m_view.setSize(m_view.getSize().y * newAspect, m_view.getSize().y);
}