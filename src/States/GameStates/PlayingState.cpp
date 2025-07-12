#include "States/GameStates/PlayingState.h"
#include "States/GameStates/MainMenuState.h"

PlayingState::PlayingState(GameController& game) : m_currentLevel(3)
{
    // Load the first level when this state begins
   game.loadLevel(m_currentLevel);
   // game.loadHUD();
}

void PlayingState::handleEvents(GameController& game)
{
    // Delegate to the controller's gameplay event handler
    game.handleEvents();
}

void PlayingState::update(float deltaTime, GameController& game)
{
    // Check for level completion
    if (game.getLevel()->getCompleted()) {
        m_currentLevel++;
        if (m_currentLevel > m_maxLevels) {
            // Game finished, go back to menu
            game.changeState(std::make_unique<MainMenuState>());
            return;
        }
        game.loadLevel(m_currentLevel);
    }

    // Delegate to the controller's gameplay update logic
    game.update(deltaTime);
}

void PlayingState::draw(GameController& game)
{
    // Delegate to the controller's gameplay drawing logic
    game.draw();
}