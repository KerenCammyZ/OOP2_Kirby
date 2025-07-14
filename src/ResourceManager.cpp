#include "ResourceManager.h"

std::shared_ptr<sf::Texture> ResourceManager::getTexture(const std::string& filename) {
    auto it = m_textures.find(filename);
    if (it != m_textures.end()) {
        return it->second;
    }

    auto texture = std::make_shared<sf::Texture>();
    if (texture->loadFromFile(filename)) {
        m_textures[filename] = texture;
        return texture;
    }

    return nullptr;
}

std::shared_ptr<sf::Font> ResourceManager::getFont(const std::string& filename) {
    auto it = m_fonts.find(filename);
    if (it != m_fonts.end()) {
        return it->second;
    }

    auto font = std::make_shared<sf::Font>();
    if (font->loadFromFile(filename)) {
        m_fonts[filename] = font;
        return font;
    }

    return nullptr;
}

std::shared_ptr<sf::SoundBuffer> ResourceManager::getSound(const std::string& filename) {
    auto it = m_sounds.find(filename);
    if (it != m_sounds.end()) {
        return it->second;
    }

    auto sound = std::make_shared<sf::SoundBuffer>();
    if (sound->loadFromFile(filename)) {
        m_sounds[filename] = sound;
        return sound;
    }

    return nullptr;
}

void ResourceManager::clear() {
    m_textures.clear();
    m_fonts.clear();
    m_sounds.clear();
}