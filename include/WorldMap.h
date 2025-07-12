// WorldMap.h
#pragma once
#include <SFML/Graphics.hpp>
#include "GameObj/GameObject.h" // Use the base class
#include <vector>
#include <memory>

class WorldMap
{
public:
	// Constructor now takes the texture for the visual background
	WorldMap(std::shared_ptr<sf::Texture> backgroundTexture);
	~WorldMap() = default;

	// Initializes the visual sprite
	void init(std::shared_ptr<sf::Texture> backgroundTexture);

	// Draws the visual background sprite
	void draw(sf::RenderTarget& target) const;

	// The main function to load all objects from a collision map file
	std::vector<std::unique_ptr<GameObject>> loadObjectsFromFile(const std::string& filePath, Kirby* kirby);

	// Getters for world properties
	sf::FloatRect getBounds() const;
	sf::Vector2f getSize() const;

private:
	// Helper to set the size of the visual sprite
	void setSize(const sf::Vector2f& size);

	std::shared_ptr<sf::Texture> m_backgroundTexture;
	sf::Sprite m_backgroundSprite;
	sf::Vector2f m_scale; // The scale factor applied to the map
};