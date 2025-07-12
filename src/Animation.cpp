// Animation.cpp
#pragma once
#include "Animation.h"
#include <iostream>

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
    std::cout << "Adding frame sequence: " << spritePrefix << " from " << startIndex << " to " << endIndex << std::endl;
    for (int i = startIndex; i <= endIndex; i++) {
        std::string spriteName = spritePrefix + std::to_string(i);
        addFrame(spriteName, frameDuration);
        std::cout << "  Added frame: " << spriteName << " (duration: " << frameDuration << ")" << std::endl;
    }
}
void Animation::update(float deltaTime) {
    if (!m_isPlaying || m_frames.empty() || m_isFinished) {
        return;
    }

    m_currentFrameTime += deltaTime * m_playbackSpeed;

    if (m_currentFrameTime >= m_frames[m_currentFrameIndex].duration) {
        m_currentFrameTime = 0.0f;
        m_currentFrameIndex++;

        if (m_currentFrameIndex >= static_cast<int>(m_frames.size())) {
            if (m_isLooping) {
                m_currentFrameIndex = 0;
            }
            else {
                m_currentFrameIndex = static_cast<int>(m_frames.size()) - 1;
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
    m_isFinished = false; // Make sure we reset finished state
    std::cout << "Animation '" << m_name << "' started playing" << std::endl;
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