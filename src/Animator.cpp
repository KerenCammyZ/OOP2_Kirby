// Animator.cpp
#include "Animator.h"
#include "ResourceManager.h"
#include <iostream>

Animator::Animator()
    : m_currentFrame(0), m_frameTimer(0.0f), m_isPlaying(false), m_isFinished(false)
{
}

bool Animator::loadSpriteSheet(const std::string& filePath) {
	m_texture = ResourceManager::getInstance().getTexture(filePath);
    if (!m_texture) {
        std::cerr << "Failed to load spritesheet: " << filePath << std::endl;
        return false;
    }
    m_sprite.setTexture(*m_texture);
    return true;
}

void Animator::addGridAnimation(const std::string& name, int startX, int startY,
    int frameWidth, int frameHeight, int frameCount,
    float frameDuration, bool loop) {
    std::vector<sf::IntRect> frames;
    for (int i = 0; i < frameCount; i++) {
        frames.push_back(sf::IntRect(startX + (i * frameWidth), startY, frameWidth, frameHeight));
    }

    AnimationData data;
    data.frames = frames;
    data.frameDuration = frameDuration;
    data.loop = loop;

    m_animations[name] = data;
}

void Animator::play(const std::string& animationName) {
    auto it = m_animations.find(animationName);
    if (it == m_animations.end()) {
        std::cerr << "Animation '" << animationName << "' not found!" << std::endl;
        return;
    }

    if (m_currentAnimationName != animationName || m_isFinished) {
        m_currentAnimationName = animationName;
        m_currentFrame = 0;
        m_frameTimer = 0.0f;
        m_isFinished = false;
        m_isPlaying = true;

        if (!it->second.frames.empty()) {
            m_sprite.setTextureRect(it->second.frames[0]);
        }
    }
}

void Animator::update(float deltaTime) {
    if (!m_isPlaying || m_isFinished) return;

    auto it = m_animations.find(m_currentAnimationName);
    if (it == m_animations.end()) return;

    const AnimationData& anim = it->second;
    if (anim.frames.empty()) return;

    m_frameTimer += deltaTime;

    if (m_frameTimer >= anim.frameDuration) {
        m_frameTimer = 0.0f;
        m_currentFrame++;

        if (m_currentFrame >= static_cast<int>(anim.frames.size())) {
            if (anim.loop) {
                m_currentFrame = 0;
            }
            else {
                m_currentFrame = static_cast<int>(anim.frames.size()) - 1;
                m_isFinished = true;
                m_isPlaying = false;
            }
        }

        m_sprite.setTextureRect(anim.frames[m_currentFrame]);
    }
}

void Animator::draw(sf::RenderTarget& target, float x, float y,
    float scaleX, float scaleY) {
    if (!m_texture) return;

    // Ensure origin is centered for collision consistency
    sf::IntRect rect = m_sprite.getTextureRect();
    m_sprite.setOrigin(rect.width / 2.0f, rect.height / 2.0f);
    m_sprite.setPosition(x, y);
    m_sprite.setScale(scaleX, scaleY);
    target.draw(m_sprite);
}