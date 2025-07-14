#pragma once
#include "States/GameStates/GameState.h"
//#include "MusicManager.h"

class MainMenuState : public GameState
{
public:
    MainMenuState(GameController& game);
    void handleEvents(GameController& game) override;
    void update(float deltaTime, GameController& game) override;
    void draw(GameController& game) override;
private:
   
    sf::Text m_title;
    sf::Text m_playButton;
	sf::Text m_helpButton;
    sf::Text m_exitButton;
    std::shared_ptr<sf::Font> m_font;
};