#include "Behaviors/SparkAttack.h"
#include "GameObj/MovingObj/Enemy.h" // To access the owner

SparkAttack::SparkAttack() : m_flashTimer(0.0f) {}

void SparkAttack::attack(float deltaTime)
{
    if (!m_owner) return;

    // This logic makes the owner's sprite flash every 0.1 seconds
    m_flashTimer += deltaTime;
    if (m_flashTimer > 0.1f)
    {
        m_flashTimer = 0.0f;

        // Get the sprite to modify it
        sf::Sprite& sprite = m_owner->getSprite(); // You will need to add this getter to Enemy.h

        // Toggle between semi-transparent yellow and white
        if (sprite.getColor() == sf::Color::White) {
            sprite.setColor(sf::Color(255, 255, 0, 200));
        }
        else {
            sprite.setColor(sf::Color::White);
        }
    }
}