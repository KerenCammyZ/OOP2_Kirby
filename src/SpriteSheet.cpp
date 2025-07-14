// SpriteSheet.cpp
#include "SpriteSheet.h"
#include <iostream>


bool SpriteSheet::loadTexture(const std::string& filePath) {
    m_texture = std::make_shared<sf::Texture>();

    if (!m_texture->loadFromFile(filePath)) {
        std::cerr << "Failed to load spritesheet: " << filePath << std::endl;
        return false;
    }

    m_tempSprite.setTexture(*m_texture);
    std::cout << "Spritesheet loaded: " << filePath
        << " (Size: " << m_texture->getSize().x << "x" << m_texture->getSize().y << ")" << std::endl;

    return true;
}

// Define a sprite type with its dimensions
void SpriteSheet::defineSpriteType(const std::string& typeName, int width, int height)
{
    // This is mainly for reference
}

// Add a specific sprite to the catalog
void SpriteSheet::addSprite(const std::string& spriteName, int x, int y, int width, int height)
{
    m_sprites[spriteName] = SpriteInfo(x, y, width, height);
}


/*
*    Quick add for grid-based sprites (like digits 0-9 in a row)
* 
*    @param typePrefix: Prefix for sprite names (e.g., "digit")
*    @param startX: Starting X position in the spritesheet
*    @param startY: Starting Y position in the spritesheet
*    @param spriteWidth: Width of each sprite
*    @param spriteHeight: Height of each sprite
*    @param count: Total number of sprites to add
*    @param columns: Number of columns in the grid (for multiple rows)
*/
void SpriteSheet::addSpriteGrid(const std::string& typePrefix, int startX, int startY,
    int spriteWidth, int spriteHeight, int count, int columns) {
    for (int i = 0; i < count; i++) {
        int col = i % columns;
        int row = i / columns;

        int x = startX + (col * spriteWidth);
        int y = startY + (row * spriteHeight);

        std::string spriteName = typePrefix + std::to_string(i);
        addSprite(spriteName, x, y, spriteWidth, spriteHeight);
    }
}

// Draw a sprite by name at a specific position
void SpriteSheet::drawSprite(sf::RenderTarget& target, const std::string& spriteName,
    float x, float y, float scaleX, float scaleY) {
    auto it = m_sprites.find(spriteName);
    if (it == m_sprites.end()) {
        std::cerr << "Sprite '" << spriteName << "' not found!" << std::endl;
        return;
    }

    setupSprite(it->second, x, y, scaleX, scaleY);
    target.draw(m_tempSprite);
}


void SpriteSheet::drawSpriteByIndex(sf::RenderTarget& target, const std::string& typePrefix,
    int index, float x, float y, float scaleX, float scaleY) {
    std::string spriteName = typePrefix + std::to_string(index);
    drawSprite(target, spriteName, x, y, scaleX, scaleY);
}


void SpriteSheet::setupSprite(const SpriteInfo& info, float x, float y, float scaleX, float scaleY) {
    if (!m_texture) {
        return;
    }

    // Set the texture rectangle to the specific sprite area
    m_tempSprite.setTextureRect(sf::IntRect(info.x, info.y, info.width, info.height));

    m_tempSprite.setPosition(x, y);
    m_tempSprite.setScale(scaleX, scaleY);
}