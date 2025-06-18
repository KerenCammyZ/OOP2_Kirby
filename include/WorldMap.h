// WorldMap.h
#pragma once

#include <SFML/Graphics.hpp>
#include "GameObj/FixedObj/StaticObject.h"	

class WorldMap
{
public:
	WorldMap(std::shared_ptr<sf::Texture>& backgroundTexture);
	~WorldMap();

	void init(std::shared_ptr<sf::Texture> backgroundTexture);
	void draw(sf::RenderTarget& target) const;
	sf::Vector2f getSize() const;
	sf::FloatRect getBounds() const;
	void setSize(const sf::Vector2f& size);
	void setScale(const sf::Vector2f& mapScale);
	void setCollisionMap(std::unique_ptr<sf::Image> collisionMap);

	std::vector<std::unique_ptr<StaticObject>> loadCollisions();

private:
	sf::Vector2f m_scale;
	std::unique_ptr<sf::Image> m_worldMap; // collision map (for internal game logic)
	std::shared_ptr<sf::Texture> m_backgroundTexture; // visual map (what the player sees)
	sf::Sprite m_backgroundSprite;
};