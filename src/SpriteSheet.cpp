// SpriteSheet.cpp
#include "SpriteSheet.h"
#include <iostream>

SpriteSheet::SpriteSheet() {
    // Constructor - nothing special needed
}

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
void SpriteSheet::defineSpriteType(const std::string& typeName, int width, int height) {
    // This is mainly for documentation/reference
    std::cout << "Defined sprite type '" << typeName << "': " << width << "x" << height << std::endl;
}

// Add a specific sprite to the catalog
void SpriteSheet::addSprite(const std::string& spriteName, int x, int y, int width, int height) {
    m_sprites[spriteName] = SpriteInfo(x, y, width, height);
    std::cout << "Added sprite '" << spriteName << "' at (" << x << "," << y << ") size " << width << "x" << height << std::endl;
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

    // Set position and scale
    m_tempSprite.setPosition(x, y);
    m_tempSprite.setScale(scaleX, scaleY);
}


// Additional Documentation:
/*
* SpriteSheet::addSpriteGrid()
* -----------------------------
* The addSpriteGrid function adds multiple sprites to the sprite catalog
* by automatically generating their names and positions based on a grid layout.
* It is useful for efficiently registering a series of similar sprites (like animation frames or digits)
* that are arranged in rows and columns within a spritesheet.
* Each sprite is named using the provided prefix and its index (e.g., "digit0", "digit1", ...),
* and their positions are calculated from the starting coordinates, sprite size, and grid dimensions.
*
* Some potential use cases for the addSpriteGrid function in game development include:
*
* •	Animation Frames: Quickly registering all frames of a character or object animation that are laid out in a grid on a spritesheet.
*
* •	UI Elements: Loading button states, icons, or other interface elements that are organized in a grid.
*
* •	Font Glyphs: Registering a set of character images (e.g., numbers, letters) for custom bitmap fonts
*
* •	Inventory Items: Many 2D scrollers include collectible items, power-ups, or equipment.
*   An inventory system lets players collect, view, and use these items.
*   The icons for these items are typically stored in a spritesheet and managed similarly to other sprites.
*/