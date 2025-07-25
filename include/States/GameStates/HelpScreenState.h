#pragma once
#include <memory>
#include <SFML/Graphics.hpp>
#include "GameController.h"

class GameController;

class HelpScreenState : public GameState
{
public:
	HelpScreenState();
    ~HelpScreenState() = default;
    void handleEvents(GameController& game) override;
    void update(float deltaTime, GameController& game) override;
    void draw(GameController& game) override;
private:
    sf::Text m_backButtonText;
    sf::RectangleShape m_backButtonRect;
	sf::Sprite m_backgroundSprite;
    std::shared_ptr<sf::Font> m_font;
	std::shared_ptr<sf::Texture> m_backgroundTexture;
};