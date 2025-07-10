// ResourceManager.h
#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <memory>
#include <string>

class ResourceManager
{
public:
	static std::shared_ptr<sf::Texture> getTexture(const std::string& filename);
private:
	static std::map<std::string, std::shared_ptr<sf::Texture>> m_textures;
};