// WorldMap.h
#pragma once

#include <SFML/Graphics.hpp>
#include "StaticObject.h"	

class WorldMap
{
public:
	WorldMap();
	~WorldMap();

	void init(std::shared_ptr<sf::Texture> backgroundTexture);
	void draw(sf::RenderTarget& target) const;
	sf::Vector2f getSize() const;
	sf::FloatRect getBounds() const;
	void setSize(const sf::Vector2f& size);

	std::vector<std::unique_ptr<StaticObject>> loadCollisions(const sf::Image& collisionMap, sf::Vector2f scale);

private:
	std::shared_ptr<sf::Texture> m_backgroundTexture;
	sf::Sprite m_backgroundSprite;
};