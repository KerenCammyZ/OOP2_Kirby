#pragma once
#include "Behaviors/MoveBehavior.h"
#include "GameObj/MovingObj/Enemy.h"

class FlyingMove : public MoveBehavior
{
public:
    void move(float deltaTime) override
    {
        // Use a persistent time accumulator to ensure smooth sinusoidal movement
        static float timeAccumulator = 0.0f;
        timeAccumulator += deltaTime;

        float amplitude = 40.0f; // Height of the sine wave
        float frequency = 2.0f;  // Oscillations per second

        // Get the owner's current position
        sf::Vector2f position = m_owner->getPosition();
		sf::Vector2f direction = m_owner->getDirection();
        float speed = m_owner->getSpeed();

        // Use the initial Y as the base for the sine wave
        static float baseY = position.y;

        // Calculate new Y position using sine wave
        float newY = baseY + amplitude * std::sin(frequency * timeAccumulator);

		// Calculate new X position based on direction and speed
        float newX = position.x + direction.x * speed * deltaTime;
        
		// Ensure the new X position wraps around the screen
        if (newX < 0) {
            newX = SCREEN_WIDTH;
        }
        else if (newX > SCREEN_WIDTH) { // when flying from left to right
            newX = 0;
        }

        // Update the object's position
        m_owner->setPosition(sf::Vector2f(newX, newY));
    }
};