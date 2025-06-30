#pragma once
#include "Behaviors/MoveBehavior.h"
#include "GameObj/MovingObj/Enemy.h"
#include <cmath> // For std::abs and std::sin

class HoppingMove : public MoveBehavior
{
public:
    HoppingMove() : m_timeAccumulator(0.0f), m_baseY(0.0f), m_isBaseYSet(false) {}

    void move(float deltaTime) override
    {
        // If this is the first time move() is called, capture the initial Y position.
        if (!m_isBaseYSet)
        {
            m_baseY = m_owner->getPosition().y;
            m_isBaseYSet = true;
        }

        // Each instance has its own time accumulator for independent movement.
        m_timeAccumulator += deltaTime;

        // --- Hopping Physics Parameters ---
        float hopHeight = 30.0f; // How high the enemy hops.
        float hopFrequency = 3.0f; // How many hops per second.

        // Get the owner's current position and properties
        sf::Vector2f position = m_owner->getPosition();
        sf::Vector2f direction = m_owner->getDirection();
        float speed = m_owner->getSpeed();

        // --- Calculate the new position ---
        // Calculate vertical position using the absolute value of a sine wave.
        // This creates a series of positive "hops" from the base Y position.
        float newY = m_baseY - hopHeight * std::abs(std::sin(hopFrequency * m_timeAccumulator));

        // Calculate horizontal position based on direction and speed.
        float newX = position.x + direction.x * speed * deltaTime;

        // Optional: Add screen wrapping logic if needed for this enemy type.
        // if (newX < 0) { newX = SCREEN_WIDTH; }
        // else if (newX > SCREEN_WIDTH) { newX = 0; }

        // Update the owner's position.
        m_owner->setPosition(sf::Vector2f(newX, newY));
    }

private:
    // Member variables ensure each enemy with this behavior moves independently.
    float m_timeAccumulator;
    float m_baseY;
    bool m_isBaseYSet;
};