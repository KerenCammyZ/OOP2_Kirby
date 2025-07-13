#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <map>
#include <string>
#include <memory>
#include <mutex>

class ResourceManager {
public:
    // Get the singleton instance
    static ResourceManager& getInstance();

    // Texture
    std::shared_ptr<sf::Texture> getTexture(const std::string& filename);

    // Font
    std::shared_ptr<sf::Font> getFont(const std::string& filename);

    // SoundBuffer
    std::shared_ptr<sf::SoundBuffer> getSoundBuffer(const std::string& filename);

    // Prevent copying/moving
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

private:
    ResourceManager() = default;
    ~ResourceManager() = default;

    std::map<std::string, std::shared_ptr<sf::Texture>> m_textures;
    std::map<std::string, std::shared_ptr<sf::Font>> m_fonts;
    std::map<std::string, std::shared_ptr<sf::SoundBuffer>> m_soundBuffers;
    std::mutex m_mutex;
};


// ----------------------------
// usage for fonts and sounds:
// -----------------------------
/*

auto font = ResourceManager::getInstance().getFont("arial.ttf");
auto sound = ResourceManager::getInstance().getSoundBuffer("jump.wav");

*/
