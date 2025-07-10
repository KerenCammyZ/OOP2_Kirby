// GameObjectFactory.h : A factory pattern implementation. allows dynamic creation of GameObject instances based on a sf::Color key.
#pragma once  
#include "GameObj/GameObject.h" // The base class for ALL created objects  
#include <SFML/Graphics.hpp>  
#include <memory>  
#include <map>  

class Kirby;  

/**  
 * A struct for comparing sf::Color objects, enabling their use as keys in a std::map.  
 * The comparison is performed by comparing the red, green, blue, and alpha channels in order. 
 * This ensures that sf::Color objects can be used as unique identifiers in the factory's registry.
 */  
struct ColorComparator {  
    bool operator()(const sf::Color& a, const sf::Color& b) const {  
        if (a.r != b.r) return a.r < b.r;  
        if (a.g != b.g) return a.g < b.g;  
        if (a.b != b.b) return a.b < b.b;  
        return a.a < b.a;  
    }  
};  

/**  
 * GameObjectFactory is responsible for creating GameObject instances based on a color key.  
 * It uses a static registry to map color keys to creation functions.
 * This allows dynamic and flexible creation of GameObjects without hardcoding their types.  
 */  
class GameObjectFactory  
{  
public:  
    /**  
     * Type alias for a function pointer that creates a GameObject.  
     * The function takes a position and a pointer to a Kirby instance as arguments.
     * It returns a unique_ptr to the created GameObject. 
     */  
    using CreateFunction = std::unique_ptr<GameObject>(*)(sf::Vector2f position, const Kirby* kirby);  

    /**  
     * Creates a GameObject based on the provided color key.  
     *  
     * @param colorKey The color key used to identify the type of GameObject to create.  
     * @param position The position where the GameObject will be created.  
     * @param kirby A pointer to the Kirby instance, used for context during creation.  
     * @return A unique_ptr to the created GameObject, or nullptr if the color key is not registered.  
     *  
     * How it works:  
     * - The function looks up the color key in the static map of registered creation functions.  
     * - If a matching function is found, it is called with the provided position and Kirby pointer.  
     * - If no matching function is found, nullptr is returned.  
     */  
    static std::unique_ptr<GameObject> create(const sf::Color& colorKey, sf::Vector2f position, const Kirby* kirby);  

    /**  
     * Registers a creation function for a specific color key.  
     *  
     * @param colorKey The color key associated with the GameObject type.  
     * @param func The function used to create the GameObject.  
     * @return True if the registration was successful, false if the color key is already registered.  
     *  
     * How it works:  
     * - The function adds the color key and its associated creation function to the static map.  
     * - If the color key already exists, the new function overwrites the existing one.  
     * - Always returns true to indicate successful registration.  
     */  
    static bool registerType(const sf::Color& colorKey, CreateFunction func);  

private:  
    /**  
     * Retrieves the static map that stores the registered creation functions.  
     *  
     * @return A reference to the static map of color keys and creation functions.  
     * 
     * How it works:  
     * - The map is declared as a static variable inside this function.  
     * - This ensures that the map is shared across all instances of GameObjectFactory.  
     * - The map uses ColorComparator to compare sf::Color keys.  
     */  
    static std::map<sf::Color, CreateFunction, ColorComparator>& getMap()  
    {  
        static std::map<sf::Color, CreateFunction, ColorComparator> m_creationFunctions;  
		return m_creationFunctions; // Returns a reference to the static map.  
    }
};
