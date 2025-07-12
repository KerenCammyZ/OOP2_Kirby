// Animator.h
#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <memory>
#include <map>

class Animator {
public:
    Animator();
    ~Animator() = default;

    bool loadSpriteSheet(const std::string& filePath);
    void addGridAnimation(const std::string& name, int startX, int startY,
        int frameWidth, int frameHeight, int frameCount,
        float frameDuration = 0.2f, bool loop = true);
    void play(const std::string& animationName);
    void update(float deltaTime);
    void draw(sf::RenderTarget& target, float x, float y,
        float scaleX = 1.0f, float scaleY = 1.0f);

    bool isPlaying() const { return m_isPlaying; }
    std::string getCurrentAnimation() const { return m_currentAnimationName; }

private:
    struct AnimationData {
        std::vector<sf::IntRect> frames;
        float frameDuration;
        bool loop;
    };

    std::shared_ptr<sf::Texture> m_texture;
    sf::Sprite m_sprite;
    std::map<std::string, AnimationData> m_animations;
    std::string m_currentAnimationName;
    int m_currentFrame;
    float m_frameTimer;
    bool m_isPlaying;
    bool m_isFinished;
};