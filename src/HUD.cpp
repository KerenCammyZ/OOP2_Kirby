// HUD.cpp
#include "HUD.h"
#include "GlobalSizes.h"
#include "GameController.h"
#include "ResourceManager.h"
#include <iostream>

HUD::HUD()
    : m_currentHealth(6), m_maxHealth(6), m_lives(5), m_score(0), m_kirbyState("normal")
{
    // Initialize display area to bottom of screen by default
    setDisplayArea(0, 0, SCREEN_WIDTH, HUD_HEIGHT);

    // Initialize spritesheet
    m_spriteSheet = std::make_unique<SpriteSheet>();
}

// Load the HUD texture from a file
bool HUD::loadTexture(const std::string& filePath) {
    m_hudTexture = ResourceManager::getInstance().getTexture(filePath);

    if (!m_hudTexture) {
		throw std::runtime_error("Failed to load HUD texture: " + filePath);
    }
    m_hudSprite.setTexture(*m_hudTexture);
    updateSprite(); // Set initial scaling and position

    return true;
}

// Load and setup the spritesheet for HUD elements
bool HUD::loadSpriteSheet(const std::string& filePath) {
    if (!m_spriteSheet->loadTexture(filePath)) {
        return false;
    }
    // Define your sprite types based on your specifications
    m_spriteSheet->defineSpriteType("digit", 8, 9);
    m_spriteSheet->defineSpriteType("state", 32, 40);
    m_spriteSheet->defineSpriteType("text", 50, 10);
    m_spriteSheet->defineSpriteType("health", 8, 15);
    m_spriteSheet->defineSpriteType("star", 13, 12);

    m_spriteSheet->addSprite("normal", 0, 42, 32, 40); 
    m_spriteSheet->addSprite("hyper", 32, 42, 32, 40); 
    m_spriteSheet->addSprite("spark", 64, 42, 32, 40);
    m_spriteSheet->addSprite("invincible", 96, 42, 32, 40);

    m_spriteSheet->addSprite("kirbyText", 0, 0, 50, 10); 
    m_spriteSheet->addSprite("scoreText", 0, 10, 50, 10);

    m_spriteSheet->addSpriteGrid("digit", 0, 33, 8, 9, 10, 10);
    m_spriteSheet->addSpriteGrid("capsule", 60, 0, 8, 15, 3, 3);

    m_spriteSheet->addSprite("kirbyIcon", 60, 19, 13, 12);

    return true;
}

void HUD::updateGameData(int health, int lives, int score, const std::string& state)
{
    m_currentHealth = health;
    m_lives = lives;
    m_score = score;
    m_kirbyState = state;
}

void HUD::draw(sf::RenderTarget& target)
{
    if (!m_hudTexture) {
        return; // No texture loaded
    }

    target.draw(m_hudSprite);

    drawState(target, 144, 8);
    drawLives(target, 187, 20);
    drawHealthBar(target, 72, 13);
    drawScore(target, m_score, 72, 32);

}

// Set the area where HUD should be drawn
void HUD::setDisplayArea(float x, float y, float width, float height)
{
    m_displayArea = sf::FloatRect(x, y, width, height);
    updateSprite();
}

void HUD::drawScore(sf::RenderTarget& target, unsigned int score, float x, float y)
{
    std::string scoreStr = std::to_string(score);

    if (scoreStr.length() > 7) {
        scoreStr = std::to_string(9999999);
        std::cerr << "Score exceeds maximum display length, resetting to 9999999." << std::endl;
    }

    sf::Vector2f scale = getHUDScale();

    while (scoreStr.length() < 7) {
        scoreStr = "0" + scoreStr;
    }

    float digitPosX = x;
    float digitPosY = y;

    for (char digit : scoreStr) {
        if (digit >= '0' && digit <= '9')
        {
            int digitValue = digit - '0';
            sf::Vector2f digitPos = hudToScreen(digitPosX, digitPosY);
            m_spriteSheet->drawSpriteByIndex(target, "digit", digitValue, digitPos.x, digitPos.y, scale.x, scale.y);
            digitPosX += 8; // each digit is 8 pixels wide
        }
    }
}

void HUD::drawState(sf::RenderTarget& target, float x, float y)
{
    sf::Vector2f scale = getHUDScale();
    sf::Vector2f statePos = hudToScreen(144, 8);
    m_spriteSheet->drawSprite(target, m_kirbyState, statePos.x, statePos.y, scale.x, scale.y);
}

void HUD::drawLives(sf::RenderTarget& target, float x, float y) {

    sf::Vector2f scale = getHUDScale();
    sf::Vector2f iconPos = hudToScreen(x, y);
    m_spriteSheet->drawSprite(target, "kirbyIcon", iconPos.x, iconPos.y, scale.x, scale.y);

    std::string livesStr = std::to_string(m_lives);

    if (livesStr.length() == 1) {
        livesStr = "0" + livesStr;
    }
    float digitPosX = 208;
    float digitPosY = 24;

    for (char digit : livesStr)
    {
        int digitValue = int(digit - '0');
        sf::Vector2f digitPos = hudToScreen(digitPosX, digitPosY);
        m_spriteSheet->drawSpriteByIndex(target, "digit", digitValue, digitPos.x, digitPos.y, scale.x, scale.y);
        digitPosX += 8;
    }
}

void HUD::drawHealthBar(sf::RenderTarget& target, float x, float y) {
    sf::Vector2f scale = getHUDScale();

    for (int i = 0; i < m_maxHealth; i++)
    {
        sf::Vector2f segmentPos = hudToScreen(x + (i * 8), y);

        // Choose which capsuleSprite to draw based on health
        std::string capsuleSprite;
        if (i < m_currentHealth) {
            capsuleSprite = "capsule1"; // full capsule
        }
        else {
            capsuleSprite = "capsule2"; // empty capsule
        }

        m_spriteSheet->drawSprite(target, capsuleSprite, segmentPos.x, segmentPos.y, scale.x, scale.y);
    }
}

// Update the sprite to stretch it across the display area
void HUD::updateSprite()
{
    if (!m_hudTexture) {
        return;
    }

    sf::Vector2f originalSize(m_hudTexture->getSize());

    // Calculate scale to stretch HUD to fill the entire display area
    float scaleX = m_displayArea.width / originalSize.x;
    float scaleY = m_displayArea.height / originalSize.y;

    m_hudSprite.setScale(scaleX, scaleY);
    m_hudSprite.setPosition(m_displayArea.left, m_displayArea.top);
}

// Convert HUD coordinates (based on original HUD size) to stretched screen coordinates
sf::Vector2f HUD::hudToScreen(float hudX, float hudY) {
    if (!m_hudTexture) {
        return sf::Vector2f(hudX, hudY);
    }

    sf::Vector2f originalSize(m_hudTexture->getSize());
    float scaleX = m_displayArea.width / originalSize.x;
    float scaleY = m_displayArea.height / originalSize.y;

    return sf::Vector2f(
        m_displayArea.left + (hudX * scaleX),
        m_displayArea.top + (hudY * scaleY)
    );
}

sf::Vector2f HUD::getHUDScale()
{
    if (!m_hudTexture) {
        return sf::Vector2f(1.0f, 1.0f); // Default scale if no texture
    }
    sf::Vector2f originalSize(m_hudTexture->getSize());
    float scaleX = m_displayArea.width / originalSize.x;
    float scaleY = m_displayArea.height / originalSize.y;

    return sf::Vector2f(scaleX, scaleY);
}