// HUD.cpp
#include "HUD.h"
#include "GlobalSizes.h"
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
    m_hudTexture = std::make_shared<sf::Texture>();

    if (!m_hudTexture->loadFromFile(filePath)) {
        std::cerr << "Failed to load HUD texture: " << filePath << std::endl;
        return false;
    }

    m_hudSprite.setTexture(*m_hudTexture);
    updateSprite(); // Set initial scaling and position

    std::cout << "HUD texture loaded successfully. Original size: "
        << m_hudTexture->getSize().x << "x" << m_hudTexture->getSize().y << std::endl;

    return true;
}


// Load and setup the spritesheet for HUD elements
bool HUD::loadSpriteSheet(const std::string& filePath) {
    if (!m_spriteSheet->loadTexture(filePath)) {
        return false;
    }

    // Define your sprite types based on your specifications
    m_spriteSheet->defineSpriteType("digit", 8, 8);
    m_spriteSheet->defineSpriteType("state", 32, 40);
    m_spriteSheet->defineSpriteType("text", 50, 10);
    m_spriteSheet->defineSpriteType("health", 8, 14);
    m_spriteSheet->defineSpriteType("star", 13, 12);

    std::cout << "Spritesheet setup complete. Add your sprite locations next." << std::endl;

    // TODO: You'll need to specify where these sprites are located in your spritesheet
    // Example setup for digits 0-9 (you'll need to adjust coordinates):
    // m_spriteSheet->addSpriteGrid("digit", 0, 0, 8, 8, 10, 10);


	m_spriteSheet->addSprite("normal", 0, 30, 32, 40); // sprite for normal state
    m_spriteSheet->addSprite("hyper", 32, 30, 32, 40); // sprite for hyper state
    //m_spriteSheet->addSprite("invincible", 64, 30, 32, 40); // sprite for invincible state

    m_spriteSheet->addSprite("kirbyText", 0, 0, 50, 10); // kirby title
    m_spriteSheet->addSprite("scoreText", 0, 10, 50, 10); // score title

	m_spriteSheet->addSpriteGrid("digit", 0, 110, 8, 8, 10, 10); // digits 0-9
    m_spriteSheet->addSpriteGrid("capsule", 100, 0, 8, 14, 3, 3);

	m_spriteSheet->addSprite("kirbyIcon", 152, 0, 13, 12); // sprite for kirby lives


    return true;
}

void HUD::updateGameData(int health, int lives, int score, const std::string& state)
{
    m_currentHealth = health;
    m_lives = lives;
    m_score = score;
    m_kirbyState = state;
}


void HUD::draw(sf::RenderTarget& target) {
    if (!m_hudTexture) {
        return; // No texture loaded
    }

    // draw background first
    target.draw(m_hudSprite);
    
    // draw state 'normal state' at HUD coordinate (577, 33)
	sf::Vector2f scale = getHUDScale();
    sf::Vector2f statePos = hudToScreen(144, 8);
    m_spriteSheet->drawSprite(target, m_kirbyState, statePos.x, statePos.y, scale.x, scale.y); // kirby state display

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


void HUD::drawLives(sf::RenderTarget& target, float x, float y) {
    // Draw number of lives
	sf::Vector2f scale = getHUDScale();

    sf::Vector2f iconPos = hudToScreen(x, y);
	m_spriteSheet->drawSprite(target, "kirbyIcon", iconPos.x, iconPos.y, scale.x, scale.y);
        
    std::string livesStr = std::to_string(m_lives);
        
    if (livesStr.length() == 1) {
        livesStr = "0" + livesStr; // Ensure two digits for single-digit lives
    }
    float digitPosX = 208;
    float digitPosY = 24;
        
    for (char digit : livesStr)
    {
        int digitValue = int(digit - '0'); // Convert char to int
        sf::Vector2f digitPos = hudToScreen(digitPosX, digitPosY);
        m_spriteSheet->drawSpriteByIndex(target, "digit", digitValue, digitPos.x, digitPos.y, scale.x, scale.y);
		digitPosX += 8; // Move to next digit position
    }
}


void HUD::drawHealthBar(sf::RenderTarget& target, float x, float y) {
    // Draw health bar segments
	sf::Vector2f scale = getHUDScale();

    for (int i = 0; i < m_maxHealth; i++)
    {
        sf::Vector2f segmentPos = hudToScreen(x + (i * 8), y);

        // Choose which capsuleSprite to draw based on health
        std::string capsuleSprite;
        if (i < m_currentHealth) {
            capsuleSprite = "capsule1"; // capsule1 = full capsule
        } // capsule0 = empty_capsule, for animated HUD ('blinking' capsules) 
        else {
            capsuleSprite = "capsule2"; // none capsule, for missing health
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

    std::cout << "HUD stretched - Scale: (" << scaleX << ", " << scaleY
        << ") Final size: " << m_displayArea.width << "x" << m_displayArea.height << std::endl;
}


sf::Vector2f HUD::hudToScreen(float hudX, float hudY) {
    // Convert HUD coordinates (based on original HUD size) to stretched screen coordinates
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