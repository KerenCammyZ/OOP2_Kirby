// Animation.h
#pragma once
#include <vector>
#include <string>
#include <memory>
#include "SpriteSheet.h"

// Represents a single frame in an animation
struct AnimationFrame {
    std::string spriteName;    // Name of the sprite in the spritesheet
    float duration;            // How long this frame should be displayed (in seconds)

    AnimationFrame(const std::string& name, float dur)
        : spriteName(name), duration(dur) {
    }
};

class Animation {
public:
    Animation(const std::string& name, bool loop = true);
    ~Animation() = default;

    // Add a frame to this animation
    void addFrame(const std::string& spriteName, float duration);

    // Add multiple frames quickly (useful for sequential sprites)
    void addFrameSequence(const std::string& spritePrefix, int startIndex,
        int endIndex, float frameDuration);

    // Update the animation (call this every frame)
    void update(float deltaTime);

    // Get the current frame's sprite name
    std::string getCurrentSpriteName() const;

    // Animation controls
    void play();
    void pause();
    void stop();
    void restart();

    // Check animation state
    bool isPlaying() const { return m_isPlaying; }
    bool isFinished() const { return m_isFinished; }
    bool isLooping() const { return m_isLooping; }

    // Set animation properties
    void setLooping(bool loop) { m_isLooping = loop; }
    void setPlaybackSpeed(float speed) { m_playbackSpeed = speed; }

    // Get animation info
    const std::string& getName() const { return m_name; }
    float getTotalDuration() const;
    int getFrameCount() const { return static_cast<int>(m_frames.size()); }

private:
    std::string m_name;
    std::vector<AnimationFrame> m_frames;

    int m_currentFrameIndex;
    float m_currentFrameTime;
    float m_playbackSpeed;

    bool m_isPlaying;
    bool m_isLooping;
    bool m_isFinished;
};