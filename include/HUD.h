// HUD.h
#pragma once
#include <SFML/Graphics.hpp>
#include "SpriteSheet.h"
#include "Animator.h"
#include <memory>

class HUD {
public:
    HUD();
    ~HUD() = default;

    // Load the HUD texture
    bool loadTexture(const std::string& filePath);

    // Load and setup the spritesheet for HUD elements
    bool loadSpriteSheet(const std::string& filePath);

    // Update HUD with current game state data
    void updateGameData(int health, int lives, int score, const std::string& kirbyState);

    // Draw the HUD stretched across the display area
    void draw(sf::RenderTarget& target);

    // Set the area where HUD should be drawn
    void setDisplayArea(float x, float y, float width, float height);

    // Example HUD element drawing functions
    void drawScore(sf::RenderTarget& target, unsigned int score, float x, float y);
    void drawState(sf::RenderTarget& target, float x, float y);
    void drawLives(sf::RenderTarget& target, float x, float y);
    void drawHealthBar(sf::RenderTarget& target, float x, float y);

private:
    // Helper to get the scale factors for sprites to match HUD stretching
    sf::Vector2f getHUDScale();

    // Helper to convert HUD coordinates to screen coordinates
    sf::Vector2f hudToScreen(float hudX, float hudY);

    // Helper method to update sprite scaling and position
    void updateSprite();

    int m_score;
    int m_lives;
    int m_currentHealth;
    int m_maxHealth;
	std::string m_kirbyState; // special powers or states ("normal", "hyper", "spark", "invincible", etc.)

    sf::FloatRect m_displayArea;

    sf::Sprite m_hudSprite;
    std::shared_ptr<sf::Texture> m_hudTexture;
    std::unique_ptr<SpriteSheet> m_spriteSheet; // Spritesheet for HUD elements    
    
};