#include "WorldMap.h"

WorldMap::WorldMap(): m_position(0.f, 0.f), m_size(0.f, 0.f) {}

WorldMap::~WorldMap() = default;

void WorldMap::init(std::shared_ptr<sf::Texture> backgroundTexture)
{
    if (!backgroundTexture)
        throw std::runtime_error("Background texture cannot be null");

    m_backgroundTexture = backgroundTexture;

    // Set size based on texture dimensions
    sf::Vector2u texSize = m_backgroundTexture->getSize();
    m_size = sf::Vector2f(static_cast<float>(texSize.x),
        static_cast<float>(texSize.y));
}

void WorldMap::draw(Renderer& renderer) const
{
    if (m_backgroundTexture)
    {
        renderer.draw(*m_backgroundTexture, m_position, m_size);
    }
    else
    {
        throw std::runtime_error("Background texture not set for WorldMap");
    }
}

void WorldMap::draw(sf::RenderTarget& target) const
{
    if (m_backgroundTexture)
    {
        sf::Sprite sprite(*m_backgroundTexture);
        sprite.setPosition(m_position);
        target.draw(sprite);
    }
    else
    {
        throw std::runtime_error("Background texture not set for WorldMap");
    }
}

sf::Vector2f WorldMap::getSize() const
{
    return m_size;
}

sf::FloatRect WorldMap::getBounds() const
{
    return sf::FloatRect(m_position, m_size);
}
