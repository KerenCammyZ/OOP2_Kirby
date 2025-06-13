// LevelManager.h
#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "StaticObject.h"


struct LevelSection
{   
	sf::FloatRect worldBounds;  // Texture coordinates for this section (defines world bounds)
    int nextSectionIndex = -1;  // Index of next section (-1 = no transition)
};

class LevelManager
{
public:
    void loadLevel(const std::string& texturePath);
    void setupSections(const std::string& texturePath);

    void setCurrentSection(int index);
    int getCurrentSection() const { return m_currentSectionIndex; }

    // Advance game to next section
    void transitionToNext();

    LevelSection& getCurrentSectionData() { return m_sections[m_currentSectionIndex]; }

private:
    std::shared_ptr<sf::Texture> m_levelTexture;
    std::vector<LevelSection> m_sections;
    int m_currentSectionIndex = 0;
    sf::Sprite m_levelSprite;
};