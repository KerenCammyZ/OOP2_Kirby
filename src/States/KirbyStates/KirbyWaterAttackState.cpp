#include "States/KirbyStates/KirbyWaterAttackState.h"
#include "States/KirbyStates/KirbySwimmingState.h"
#include "GameObj/MovingObj/Kirby.h"
#include "GameObj/MovingObj/Enemy.h"
#include <SFML/Graphics.hpp>
#include <iostream>

KirbyWaterAttackState::KirbyWaterAttackState(Kirby& kirby, std::vector<std::unique_ptr<Enemy>>& enemies)
    : m_enemies(enemies), m_stateDuration(0.3f)
{
    m_waterStream = std::make_unique<Animator>();
    if (m_waterStream->loadSpriteSheet("KirbySpriteSheet.png")) {
        // Use your actual frame size and position
        m_waterStream->addGridAnimation("water_attack", 16, 111, 16, 16, 3, 0.15f, true);
        m_waterStream->play("water_attack");
    }
}

void KirbyWaterAttackState::enter(Kirby& kirby)
{
    kirby.setVelocity({ 0.f, kirby.getVelocity().y });
   // kirby.setAnimation("water_attack");
}

std::unique_ptr<KirbyState> KirbyWaterAttackState::handleInput(Kirby& kirby)
{
    if (m_stateDuration <= 0.f)
        return std::make_unique<KirbySwimmingState>();
    return nullptr;
}

void KirbyWaterAttackState::update(Kirby& kirby, float deltaTime)
{
    m_stateDuration -= deltaTime;

    // Position the animation in front of Kirby
    sf::Vector2f position = kirby.getPosition();
    float animFrameWidth = 16.f;
    float animFrameHeight = 16.f;
    float scaleX = ENTITY_SIZE / animFrameWidth;
    float scaleY = ENTITY_SIZE / animFrameHeight;

    float offsetX = (kirby.getFacingDirection() == FacingDirection::Right)
        ? (kirby.getSize().x / 2.f + animFrameWidth * scaleX / 2.f)
        : (-kirby.getSize().x / 2.f - animFrameWidth * scaleX / 2.f);
    float offsetY = 0.f;

    m_streamPosition = sf::Vector2f(position.x + offsetX, position.y);

    if (m_waterStream)
        m_waterStream->update(deltaTime);

    // Calculate the animation's global bounds for collision
    sf::FloatRect animBounds(
        m_streamPosition.x - (animFrameWidth * scaleX) / 2.f,
        m_streamPosition.y - (animFrameHeight * scaleY) / 2.f,
        animFrameWidth * scaleX,
        animFrameHeight * scaleY
    );

    if (kirby.isFacingLeft())
        m_streamDirection = -1;
    else
        m_streamDirection = 1;

    for (auto& enemy : m_enemies) {
        if (enemy && !enemy->isSwallowed() && enemy->getState() != EnemyState::BEING_SWALLOWED) {
            if (animBounds.intersects(enemy->getBounds())) {
                enemy->onSwallowed();
            }
        }
    }
}

void KirbyWaterAttackState::draw(sf::RenderTarget& target)
{
    if (m_waterStream) {
        // Scale to Kirby's size
        float animFrameWidth = 16.f;
        float animFrameHeight = 16.f;
        float scaleX = (ENTITY_SIZE / animFrameWidth) * m_streamDirection;
        float scaleY = ENTITY_SIZE / animFrameHeight;

        m_waterStream->draw(target, m_streamPosition.x, m_streamPosition.y, scaleX, scaleY);
    }
}