#pragma once
#include "States/GameStates/GameState.h"

class PlayingState : public GameState
{
public:
    PlayingState() = default;
    PlayingState(GameController& game);

    void handleEvents(GameController& game) override;
    void update(float deltaTime, GameController& game) override;
    void draw(GameController& game) override;

private:
    int m_currentLevel;
    const int m_maxLevels = 3;
};