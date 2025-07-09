#include "GameObjectFactory.h"


bool GameObjectFactory::registerType(const sf::Color& colorKey, CreateFunction func)
{
    // Registers a new type of GameObject creation function in the factory.
    // The 'colorKey' acts as a unique identifier for the type of GameObject to be created.
    // The 'func' parameter is a function pointer that defines how to create the GameObject.

    // Adds the color key and its associated creation function to the factory's internal map.
    // 'getMap()' retrieves the static map that stores these associations.
    // The map uses 'colorKey' as the key and 'func' as the value.

	getMap().emplace(colorKey, func); // emplace is used to insert the key-value pair into the map.

    // Always returns true to indicate successful registration.
    // This function does not perform any validation or error checking.
    return true;
}


std::unique_ptr<GameObject> GameObjectFactory::create(const sf::Color& colorKey, sf::Vector2f position, const Kirby* kirby)
{
    // Retrieve the map of registered GameObject creation functions.
    // Each entry in the map associates a color key with a function that creates a specific type of GameObject.
    auto it = getMap().find(colorKey);

    // Check if the color key exists in the map.
    // If it exists, 'it' points to the corresponding entry in the map.
    if (it != getMap().end())
    {
        // Call the creation function associated with the color key.
        // The function takes the position and a pointer to a Kirby object as arguments
        // and returns a unique_ptr to the newly created GameObject.
        return it->second(position, kirby);
    }

    // If the color key is not found in the map, return a nullptr.
    // This indicates that no GameObject creation function is registered for the given color key.
    return nullptr;
}