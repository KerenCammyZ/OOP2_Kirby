// SpriteSheet.h
#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <map>
#include <string>

// Struct to define a sprite's location and size in the spritesheet
struct SpriteInfo {
    int x, y;           // Position in spritesheet
    int width, height;  // Size of the sprite

    SpriteInfo(int x = 0, int y = 0, int w = 0, int h = 0)
        : x(x), y(y), width(w), height(h) {
    }
};

class SpriteSheet {
public:
    SpriteSheet() = default;
    ~SpriteSheet() = default;

    // Load the spritesheet texture
    bool loadTexture(const std::string& filePath);

    // Define sprite types with their dimensions
    void defineSpriteType(const std::string& typeName, int width, int height);

    // Add a specific sprite to the catalog
    void addSprite(const std::string& spriteName, int x, int y, int width, int height);

    // Quick add for grid-based sprites (like digits 0-9 in a row)
    void addSpriteGrid(const std::string& typePrefix, int startX, int startY,
        int spriteWidth, int spriteHeight, int count, int columns = 10);

    // Draw a sprite by name at a specific position
    void drawSprite(sf::RenderTarget& target, const std::string& spriteName,
        float x, float y, float scaleX = 1.0f, float scaleY = 1.0f);

    // Draw a sprite by type and index (useful for digits, etc.)
    void drawSpriteByIndex(sf::RenderTarget& target, const std::string& typePrefix,
        int index, float x, float y, float scaleX = 1.0f, float scaleY = 1.0f);

private:
    std::shared_ptr<sf::Texture> m_texture;
    std::map<std::string, SpriteInfo> m_sprites;
    sf::Sprite m_tempSprite; // Reusable sprite for drawing

    // Helper to set up the sprite for drawing
    void setupSprite(const SpriteInfo& info, float x, float y, float scaleX, float scaleY);
};