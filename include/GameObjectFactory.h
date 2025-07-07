#pragma once
#include "GameObj/GameObject.h" // The base class for ALL created objects
#include <SFML/Graphics.hpp>
#include <memory>
#include <map>

class Kirby;

// A struct for comparing sf::Color, so it can be a map key.
struct ColorComparator {
    bool operator()(const sf::Color& a, const sf::Color& b) const {
        if (a.r != b.r) return a.r < b.r;
        if (a.g != b.g) return a.g < b.g;
        if (a.b != b.b) return a.b < b.b;
        return a.a < b.a;
    }
};

class GameObjectFactory
{
public:
    using CreateFunction = std::unique_ptr<GameObject>(*)(sf::Vector2f position, const Kirby* kirby);

    static std::unique_ptr<GameObject> create(const sf::Color& colorKey, sf::Vector2f position, const Kirby* kirby);
    static bool registerType(const sf::Color& colorKey, CreateFunction func);

private:
    static std::map<sf::Color, CreateFunction, ColorComparator>& getMap()
    {
        static std::map<sf::Color, CreateFunction, ColorComparator> m_creationFunctions;
        return m_creationFunctions;
	}
};
