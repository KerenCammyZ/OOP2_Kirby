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

// Animation Manager - handles multiple animations for a single entity
class AnimationManager {
public:
    AnimationManager(std::shared_ptr<SpriteSheet> spriteSheet);
    ~AnimationManager() = default;

    // Add an animation to the manager
    void addAnimation(std::unique_ptr<Animation> animation);

    // Create and add an animation in one step
    Animation* createAnimation(const std::string& name, bool loop = true);

    // Switch to a different animation
    bool playAnimation(const std::string& animationName, bool restart = false);

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

private:
    std::shared_ptr<SpriteSheet> m_spriteSheet;
    std::map<std::string, std::unique_ptr<Animation>> m_animations;
    Animation* m_currentAnimation;
    std::string m_currentAnimationName;
};