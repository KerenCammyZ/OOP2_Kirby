#include "AnimationManager.h"
#include <iostream>

AnimationManager::AnimationManager()
    : m_currentAnimation(nullptr)
{
    // Don't initialize spritesheet here - it will be set later
}

void AnimationManager::initialize(std::shared_ptr<SpriteSheet> spriteSheet) {
    m_spriteSheet = spriteSheet;
}

void AnimationManager::addAnimation(std::unique_ptr<Animation> animation) {
    if (!animation) {
        std::cerr << "Warning: Trying to add null animation!" << std::endl;
        return;
    }

    std::string name = animation->getName();
    m_animations[name] = std::move(animation);
}

Animation* AnimationManager::createAnimation(const std::string& name, bool loop) {
    auto animation = std::make_unique<Animation>(name, loop);
    Animation* animPtr = animation.get();
    addAnimation(std::move(animation));
    return animPtr;
}

bool AnimationManager::playAnimation(const std::string& animationName, bool restart) {
    auto it = m_animations.find(animationName);
    if (it == m_animations.end()) {
        std::cerr << "Animation '" << animationName << "' not found!" << std::endl;
        return false;
    }

    // If we're already playing this animation and don't want to restart, do nothing
    if (m_currentAnimation == it->second.get() && !restart) {
        return true;
    }

    // Stop current animation if there is one
    if (m_currentAnimation) {
        m_currentAnimation->pause();
    }

    // Switch to the new animation
    m_currentAnimation = it->second.get();
    m_currentAnimationName = animationName;

    if (restart) {
        m_currentAnimation->restart();
    }
    else {
        m_currentAnimation->play();
    }

    return true;
}

bool AnimationManager::playAnimationSmooth(const std::string& animationName, bool restart)
{
    // If we're already playing this animation, don't restart it
    if (m_currentAnimationName == animationName && m_currentAnimation && m_currentAnimation->isPlaying()) {
        return true; // Already playing this animation
    }

    // Otherwise, play the new animation
    return playAnimation(animationName, false); // Don't force restart
}

void AnimationManager::update(float deltaTime) {
    if (m_currentAnimation) {
        m_currentAnimation->update(deltaTime);
    }
}

void AnimationManager::draw(sf::RenderTarget& target, float x, float y,
    float scaleX, float scaleY) {
    if (!m_currentAnimation || !m_spriteSheet) {
        return;
    }

    std::string spriteName = m_currentAnimation->getCurrentSpriteName();
    if (!spriteName.empty()) {
        m_spriteSheet->drawSprite(target, spriteName, x, y, scaleX, scaleY);
    }
}

const std::string& AnimationManager::getCurrentAnimationName() const {
    return m_currentAnimationName;
}

std::string AnimationManager::getCurrentSpriteName() const {
    if (m_currentAnimation) {
        return m_currentAnimation->getCurrentSpriteName();
    }
    return "";
}

bool AnimationManager::hasAnimation(const std::string& name) const {
    return m_animations.find(name) != m_animations.end();
}