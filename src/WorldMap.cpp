#include "WorldMap.h"
#include <iostream>
WorldMap::WorldMap() 
{
	m_backroundMap = std::make_unique<sf::Texture>();
};

//WorldMap::~WorldMap() {};

void WorldMap::draw(sf::RenderTarget& target) const
{
	std::cout << "Drawing WorldMap" << std::endl;
	sf::Sprite visualArea;
	visualArea.setTexture(*m_backroundMap);
	visualArea.setTextureRect(sf::IntRect(0, 0, 1016, 169));
}

void WorldMap::loadCollisionImage(const std::string& fileName)
{
	m_collisionMap->loadFromFile(fileName);
}

void WorldMap::loadBackground(const std::string& fileName)
{
	if(m_backroundMap->loadFromFile(fileName))
		std::cout << "Loading background map from: " << fileName << std::endl;
	else
		std::cout << "Failed to load background map from: " << fileName << std::endl;
}


