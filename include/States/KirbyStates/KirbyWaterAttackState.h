#pragma once
#include "States/KirbyStates/KirbyState.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Animator.h"

class Enemy;

class KirbyWaterAttackState : public KirbyState
{
public:
    KirbyWaterAttackState(Kirby& kirby, std::vector<std::unique_ptr<Enemy>>& enemies);

    void enter(Kirby& kirby) override;
    std::unique_ptr<KirbyState> handleInput(Kirby& kirby) override;
    void update(Kirby& kirby, float deltaTime) override;
    void draw(sf::RenderTarget& target) override;

private:
    std::vector<std::unique_ptr<Enemy>>& m_enemies;
    std::unique_ptr<Animator> m_waterStream;
    sf::Vector2f m_streamPosition;
    int m_streamDirection = 1;
    float m_stateDuration;
};