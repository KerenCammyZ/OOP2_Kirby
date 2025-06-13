// LevelManager.cpp
#include "LevelManager.h"
#include "StaticObject.h"
#include "Floor.h"
#include <iostream>

void LevelManager::loadLevel(const std::string& texturePath) {  
  // Load the texture for the level  
  m_levelTexture = std::make_shared<sf::Texture>();  
  if (!m_levelTexture->loadFromFile(texturePath)) {  
      throw std::runtime_error("Failed to load texture: " + texturePath);  
  }  
  m_levelSprite.setTexture(*m_levelTexture);  

  // Set up sections dynamically based on the texture  
  setupSections(texturePath);  

  // Start with the first section of the new level  
  setCurrentSection(0);  
}

void LevelManager::setupSections(const std::string& texturePath) {  
    
    // clear previous data
    m_sections.clear();  

   // Define sections Based on Level1.png image
   if (texturePath == "Level1.png") {  

       // first strip
       LevelSection section0;  
       section0.worldBounds = sf::FloatRect(0, 0, 1016, 170);  
       section0.nextSectionIndex = 1;  
       m_sections.push_back(section0);  

       // second strip
       LevelSection section1;  
       section1.worldBounds = sf::FloatRect(0, 170, 1016, 170);  
       section1.nextSectionIndex = 2;  
       m_sections.push_back(section1);  

       // third strip
       LevelSection section2;  
       section2.worldBounds = sf::FloatRect(0, 340, 1016, 170);  
       section2.nextSectionIndex = 3;  
       m_sections.push_back(section2);

   } else if (texturePath == "Level1map.png") {  
       // Define sections for Level1map.png  

       // Level 1: Room A
       LevelSection room1;
       room1.worldBounds = sf::FloatRect(16, 40, LEVEL1_WIDTH, LEVEL1_HEIGHT);
       room1.nextSectionIndex = 1;
       m_sections.push_back(room1);

       // Level 2: Room B
       LevelSection room2;
       room2.worldBounds = sf::FloatRect(16, 264, LEVEL1_WIDTH, LEVEL1_HEIGHT);
       room2.nextSectionIndex = 2;
       m_sections.push_back(room2);

       // Level 3: Room C
       LevelSection room3;
       room3.worldBounds = sf::FloatRect(16, 488, LEVEL1_WIDTH, LEVEL1_HEIGHT);
       room3.nextSectionIndex = 3;
       m_sections.push_back(room3);

   } else {  
       throw std::runtime_error("Unknown texture: " + texturePath);  
   }  
}

void LevelManager::setCurrentSection(int index) {
    if (index >= 0 && index < m_sections.size()) {
        m_currentSectionIndex = index;

        std::cout << "Transitioned to section " << index << std::endl;
    }
}

void LevelManager::transitionToNext() {
    LevelSection& current = m_sections[m_currentSectionIndex];
    if (current.nextSectionIndex != -1) {
        setCurrentSection(current.nextSectionIndex);
    }
}
