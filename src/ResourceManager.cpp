#include "ResourceManager.h"

ResourceManager& ResourceManager::getInstance() {
    static ResourceManager instance;
    return instance;
}

std::shared_ptr<sf::Texture> ResourceManager::getTexture(const std::string& filename) {
    std::lock_guard<std::mutex> lock(m_mutex);

    auto it = m_textures.find(filename);
    if (it != m_textures.end()) {
        return it->second;
    }

    auto texture = std::make_shared<sf::Texture>();
    if (!texture->loadFromFile(filename)) {
        throw std::runtime_error("Failed to load texture: " + filename);
    }
    m_textures[filename] = texture;
    return texture;
}

std::shared_ptr<sf::Font> ResourceManager::getFont(const std::string& filename) {
    std::lock_guard<std::mutex> lock(m_mutex);
    auto it = m_fonts.find(filename);
    if (it != m_fonts.end()) return it->second;
    auto font = std::make_shared<sf::Font>();
    if (!font->loadFromFile(filename))
        throw std::runtime_error("Failed to load font: " + filename);
    m_fonts[filename] = font;
    return font;
}

std::shared_ptr<sf::SoundBuffer> ResourceManager::getSoundBuffer(const std::string& filename) {
    std::lock_guard<std::mutex> lock(m_mutex);
    auto it = m_soundBuffers.find(filename);
    if (it != m_soundBuffers.end()) return it->second;
    auto buffer = std::make_shared<sf::SoundBuffer>();
    if (!buffer->loadFromFile(filename))
        throw std::runtime_error("Failed to load sound buffer: " + filename);
    m_soundBuffers[filename] = buffer;
    return buffer;
}