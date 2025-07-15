// WorldMap.h
#pragma once
#include <SFML/Graphics.hpp>
#include "GameObj/GameObject.h"
#include <vector>
#include <memory>

class WorldMap
{
public:
	WorldMap(std::shared_ptr<sf::Texture> backgroundTexture);
	~WorldMap() = default;

	void init(std::shared_ptr<sf::Texture> backgroundTexture);
	void draw(sf::RenderTarget& target) const;

	// Load the game objects from the collision map file
	std::vector<std::unique_ptr<GameObject>> loadObjectsFromFile(const std::string& filePath, Kirby* kirby);

	// Getters for world properties
	sf::FloatRect getBounds() const;
	sf::Vector2f getSize() const;

private:
	// Helper to set the size of the visual sprite
	void setSize(const sf::Vector2f& size);

	sf::Vector2f m_scale; // The scale factor applied to the map
	sf::Sprite m_backgroundSprite;
	std::shared_ptr<sf::Texture> m_backgroundTexture;
};