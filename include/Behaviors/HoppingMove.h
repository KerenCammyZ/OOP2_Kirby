#pragma once
#include "Behaviors/MoveBehavior.h"
#include "GameObj/MovingObj/Enemy.h"
#include <corecrt_math_defines.h>
#include <cmath> // For std::sin

class HoppingMove : public MoveBehavior
{
public:
    HoppingMove() :
        m_currentState(State::Waiting),
        m_timeAccumulator(0.0f),
        m_waitTimer(2.0f), // Start with an initial 2-second wait
        m_baseY(0.0f),
        m_isBaseYSet(false)
    {
    }

    void move(float deltaTime) override
    {
        if (!m_owner) return;

        // If this is the first time move() is called, capture the initial Y position.
        if (!m_isBaseYSet)
        {
            m_baseY = m_owner->getPosition().y;
            m_isBaseYSet = true;
        }

        // --- State Machine Logic ---
        switch (m_currentState)
        {
        case State::Waiting:
        {
            // In the waiting state, the enemy is on the ground.
            m_owner->setGrounded(true);

            // Countdown the wait timer.
            m_waitTimer -= deltaTime;
            if (m_waitTimer <= 0.0f)
            {
                // Wait is over, switch to the hopping state.
                m_currentState = State::Hopping;
                m_timeAccumulator = 0.0f; // Reset hop timer
            }
            break;
        }

        case State::Hopping:
        {
            // During a hop, the enemy is in the air.
            m_owner->setGrounded(false);

            // Update the hop's progress.
            m_timeAccumulator += deltaTime;

            // --- Physics Parameters ---
            const float hopHeight = 30.0f;
            const float hopFrequency = 3.0f;
            const float hopDuration = (float)M_PI / hopFrequency; // The time it takes to complete one arc

            // --- Calculate Position ---
            sf::Vector2f position = m_owner->getPosition();
            sf::Vector2f direction = m_owner->getDirection();
            float speed = m_owner->getSpeed();

            // Calculate vertical position using a single sine wave arc (0 to PI).
            float newY = m_baseY - hopHeight * std::sin(hopFrequency * m_timeAccumulator);
            float newX = position.x + direction.x * speed * deltaTime;

            m_owner->setPosition(sf::Vector2f(newX, newY));

            // Check if the hop is finished.
            if (m_timeAccumulator >= hopDuration)
            {
                // Hop is over, switch back to the waiting state.
                m_currentState = State::Waiting;
                m_waitTimer = 2.0f; // Reset the 2-second wait timer

                // Snap back to the ground perfectly to avoid floating point errors.
                m_owner->setPosition(sf::Vector2f(newX, m_baseY));
            }
            break;
        }
        }
    }

private:
    // Enum to manage the internal state of the behavior
    enum class State { Hopping, Waiting };

    State m_currentState;
    float m_timeAccumulator; // Timer for the hop's progress
    float m_waitTimer;       // Timer for the waiting period
    float m_baseY;
    bool  m_isBaseYSet;
};