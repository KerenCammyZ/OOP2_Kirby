// Animation.cpp
#include "Animation.h"
#include <iostream>

// ==================== Animation Class ====================

Animation::Animation(const std::string& name, bool loop)
    : m_name(name), m_currentFrameIndex(0), m_currentFrameTime(0.0f),
    m_playbackSpeed(1.0f), m_isPlaying(false), m_isLooping(loop), m_isFinished(false)
{
}

void Animation::addFrame(const std::string& spriteName, float duration) {
    m_frames.emplace_back(spriteName, duration);
}

void Animation::addFrameSequence(const std::string& spritePrefix, int startIndex,
    int endIndex, float frameDuration) {
    for (int i = startIndex; i <= endIndex; i++) {
        std::string spriteName = spritePrefix + std::to_string(i);
        addFrame(spriteName, frameDuration);
    }
}

void Animation::update(float deltaTime) {
    if (!m_isPlaying || m_frames.empty() || m_isFinished) {
        return;
    }

    m_currentFrameTime += deltaTime * m_playbackSpeed;

    // Check if we need to advance to the next frame
    if (m_currentFrameTime >= m_frames[m_currentFrameIndex].duration) {
        m_currentFrameTime = 0.0f;
        m_currentFrameIndex++;

        // Check if we've reached the end of the animation
        if (m_currentFrameIndex >= static_cast<int>(m_frames.size())) {
            if (m_isLooping) {
                m_currentFrameIndex = 0; // Loop back to the beginning
            }
            else {
                m_currentFrameIndex = static_cast<int>(m_frames.size()) - 1; // Stay on last frame
                m_isFinished = true;
                m_isPlaying = false;
            }
        }
    }
}

std::string Animation::getCurrentSpriteName() const {
    if (m_frames.empty()) {
        return "";
    }
    return m_frames[m_currentFrameIndex].spriteName;
}

void Animation::play() {
    m_isPlaying = true;
}

void Animation::pause() {
    m_isPlaying = false;
}

void Animation::stop() {
    m_isPlaying = false;
    m_currentFrameIndex = 0;
    m_currentFrameTime = 0.0f;
    m_isFinished = false;
}

void Animation::restart() {
    stop();
    play();
}

float Animation::getTotalDuration() const {
    float total = 0.0f;
    for (const auto& frame : m_frames) {
        total += frame.duration;
    }
    return total;
}

// ==================== AnimationManager Class ====================

AnimationManager::AnimationManager(std::shared_ptr<SpriteSheet> spriteSheet)
    : m_spriteSheet(spriteSheet), m_currentAnimation(nullptr)
{
}

void AnimationManager::addAnimation(std::unique_ptr<Animation> animation) {
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