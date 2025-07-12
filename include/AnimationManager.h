// Animation Manager.h : handles multiple animations for a single entity
#pragma once
#include "Animation.h"
#include <map>

class AnimationManager {
public:
    AnimationManager();
    ~AnimationManager() = default;

    // Initialize with spritesheet (call this after construction)
    void initialize(std::shared_ptr<SpriteSheet> spriteSheet);

    // Add an animation to the manager
    void addAnimation(std::unique_ptr<Animation> animation);

    // Create and add an animation in one step
    Animation* createAnimation(const std::string& name, bool loop = true);

    // Switch to a different animation
    bool playAnimation(const std::string& animationName, bool restart = false);
    bool playAnimationSmooth(const std::string& animationName, bool restart = false);

    // Update the current animation
    void update(float deltaTime);

    // Draw the current animation frame
    void draw(sf::RenderTarget& target, float x, float y,
        float scaleX = 1.0f, float scaleY = 1.0f);

    // Get current animation info
    Animation* getCurrentAnimation() const { return m_currentAnimation; }
    const std::string& getCurrentAnimationName() const;
    std::string getCurrentSpriteName() const;

    // Check if a specific animation exists
    bool hasAnimation(const std::string& name) const;

    // Check if properly initialized
    bool isInitialized() const { return m_spriteSheet != nullptr; }

private:
    std::shared_ptr<SpriteSheet> m_spriteSheet;
    std::map<std::string, std::unique_ptr<Animation>> m_animations;
    Animation* m_currentAnimation;
    std::string m_currentAnimationName;
};