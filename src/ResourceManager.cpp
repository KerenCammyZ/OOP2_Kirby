#include "ResourceManager.h"

std::map<std::string, std::shared_ptr<sf::Texture>> ResourceManager::m_textures;

std::shared_ptr<sf::Texture> ResourceManager::getTexture(const std::string& filename)
{
	auto it = m_textures.find(filename);
	if (it != m_textures.end())
		return it->second;

	auto texture = std::make_shared<sf::Texture>();
	if (!texture->loadFromFile(filename)) {
		// Fallback: try to load a default texture
		if (filename != "Default.png")
			return getTexture("Default.png");
		return nullptr;
	}
	m_textures[filename] = texture;
	return texture;
}