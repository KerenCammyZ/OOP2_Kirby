#pragma once
#include "GameObj/GameObject.h" // The base class for ALL created objects
#include <SFML/Graphics.hpp>
#include <memory>
#include <map>

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
    static GameObjectFactory& instance();

    // The create function now returns the BASE class pointer
    using CreateFunction = std::unique_ptr<GameObject>(*)(sf::Vector2f position);

    bool registerType(const sf::Color& colorKey, CreateFunction func);
    std::unique_ptr<GameObject> create(const sf::Color& colorKey, sf::Vector2f position) const;

private:
    GameObjectFactory() = default;
    GameObjectFactory(const GameObjectFactory&) = delete;
    void operator=(const GameObjectFactory&) = delete;

    std::map<sf::Color, CreateFunction, ColorComparator> m_creationFunctions;
};
