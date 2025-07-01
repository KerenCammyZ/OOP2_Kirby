#include "GameObjectFactory.h"
#include <iostream> // Included for logging/debugging purposes

// This is the implementation of the Singleton's instance getter.
// The 'static' keyword here is crucial. It ensures that the 'instance'
// variable is created only once, the very first time this function is called.
// Every subsequent call will return a reference to that same, single instance.
// 
/* --- NOTE: code was modified to remove the singleton pattern ---
   ---      take into consideration when reading documentation ---
*/



//GameObjectFactory& GameObjectFactory::instance()
//{
//    static GameObjectFactory instance;
//    return instance;
//}

// This function registers a "blueprint" (a creation function) in our map.
// It takes the key (the color) and the function pointer as arguments.
bool GameObjectFactory::registerType(const sf::Color& colorKey, CreateFunction func)
{
    // m_creationFunctions is the map defined in the header.
    // .emplace() is an efficient way to insert a new key-value pair.
    // Here, we're mapping the colorKey to the provided creation function 'func'.
    getMap().emplace(colorKey, func);

    // We return true to confirm the registration was successful. This is what
    // allows the 'static bool isRegistered = ...' trick to work.
    return true;
}

// This is the core creation logic. Given a key (a color), it builds the object.
std::unique_ptr<GameObject> GameObjectFactory::create(const sf::Color& colorKey, sf::Vector2f position , const Kirby* kirby)
{
    // We use .find() to look for the colorKey in our map of blueprints.
    // .find() returns an "iterator" to the key-value pair if it's found.
    auto it = getMap().find(colorKey);

    // Check if the iterator is valid (i.e., it's not pointing to the end of the map,
    // which is what .find() returns when the key doesn't exist).
    if (it != getMap().end())
    {
        // If we found a blueprint for this color:
        // 'it->second' accesses the value part of the key-value pair, which is our
        // stored creation function (the lambda).
        // We then call that function, passing the 'position' argument it needs.
        // This executes the 'return std::make_unique<...>()' inside the lambda.
        return it->second(position, kirby);
    }

    // If no blueprint was found in the map for this specific color,
    // we return a nullptr to signal that no object could be created.
    // This is important so the calling code (like WorldMap) can safely ignore it.
    return nullptr;
}