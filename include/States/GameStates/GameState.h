#pragma once
#include <memory>
#include <SFML/Graphics.hpp>
#include "GameController.h"

class GameController;

class GameState
{
public:
    virtual ~GameState() = default;
    virtual void handleEvents(GameController& game) = 0;
    virtual void update(float deltaTime, GameController& game) = 0;
    virtual void draw(GameController& game) = 0;
private:

};