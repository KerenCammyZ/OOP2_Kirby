#pragma once
#include "States/GameStates/GameState.h"

class MainMenuState : public GameState
{
public:
    MainMenuState();
    void handleEvents(GameController& game) override;
    void update(float deltaTime, GameController& game) override;
    void draw(GameController& game) override;
private:
    //TODO: Move to Button class
    sf::Font m_font;
    sf::Text m_title;
    sf::Text m_playButton;
    sf::Text m_exitButton;
};