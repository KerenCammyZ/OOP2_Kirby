// ResourceManager.h
#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <map>
#include <memory>
#include <string>

class ResourceManager {
public:
    static ResourceManager& getInstance() {
        static ResourceManager instance;
        return instance;
    }

    std::shared_ptr<sf::Texture> getTexture(const std::string& filename);
    std::shared_ptr<sf::Font> getFont(const std::string& filename);
    std::shared_ptr<sf::SoundBuffer> getSound(const std::string& filename);

    // Simple cleanup
    void clear();

private:
    ResourceManager() = default;
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

    std::map<std::string, std::shared_ptr<sf::Texture>> m_textures;
    std::map<std::string, std::shared_ptr<sf::Font>> m_fonts;
    std::map<std::string, std::shared_ptr<sf::SoundBuffer>> m_sounds;
};