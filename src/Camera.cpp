#include "Camera.h"
#include <algorithm> // For std::max and std::min

Camera::Camera(const sf::Vector2f& center, const sf::Vector2f& size)
{
    m_view.setCenter(center);
    m_view.setSize(size);
}

void Camera::setSize(float width, float height)
{
    m_view.setSize(width, height);
}

void Camera::setSize(const sf::Vector2f& size)
{
    m_view.setSize(size);
}

void Camera::setCenter(const sf::Vector2f& center)
{
    m_view.setCenter(center);
}

const sf::View& Camera::getView() const
{
    return m_view;
}

void Camera::clampToArea(const sf::FloatRect& worldBounds)
{
    sf::Vector2f center = m_view.getCenter();
    sf::Vector2f size = m_view.getSize();
    sf::Vector2f halfSize = size / 2.0f;

    // Calculate the valid range for the camera's center based on world bounds.
    // The camera's center cannot go so far left that the left edge of the view
    // is to the left of the world's left edge. So, the minimum center.x is
    // the world's left edge plus half the view's width.
    float leftLimit = worldBounds.left + halfSize.x;
    float rightLimit = worldBounds.left + worldBounds.width - halfSize.x;
    float topLimit = worldBounds.top + halfSize.y;
    float bottomLimit = worldBounds.top + worldBounds.height - halfSize.y;

    // Edge Case: If the map is smaller than the view, the limits will be inverted.
    // In this case, we should just center the view on the map.
    if (worldBounds.width < size.x)
    {
        center.x = worldBounds.left + worldBounds.width / 2.0f;
    }
    else
    {
        // Clamp the center coordinates within the valid range.
        center.x = std::max(leftLimit, std::min(center.x, rightLimit));
    }

    if (worldBounds.height < size.y)
    {
        center.y = worldBounds.top + worldBounds.height / 2.0f;
    }
    else
    {
        center.y = std::max(topLimit, std::min(center.y, bottomLimit));
    }

    // Apply the clamped center position to the view.
    m_view.setCenter(center);
}