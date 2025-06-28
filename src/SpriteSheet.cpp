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