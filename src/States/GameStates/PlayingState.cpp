#include "States/GameStates/PlayingState.h"
#include "States/GameStates/MainMenuState.h"

PlayingState::PlayingState(GameController& game) : m_currentLevel(1)
{
   game.loadLevel(m_currentLevel);
}

void PlayingState::handleEvents(GameController& game)
{
    game.handleEvents();
}

void PlayingState::update(float deltaTime, GameController& game)
{
    // Check for level completion
    if (game.getLevel()->getCompleted()) {
        m_currentLevel++;
        if (m_currentLevel > m_maxLevels) {
            // Game finished, go back to menu
            game.changeGameState(std::make_unique<MainMenuState>(game));
            return;
        }
        game.loadLevel(m_currentLevel);
    }
    game.update(deltaTime);
}

void PlayingState::draw(GameController& game)
{
    game.draw();
}