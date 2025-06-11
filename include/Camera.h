#include <SFML/Graphics.hpp>

class Camera
{
public:
    // Constructor now takes an initial center and size (in pixels)
    Camera(const sf::Vector2f& center, const sf::Vector2f& size);
    ~Camera() = default;

    // Sets the size of the view (e.g., to match the window)
    void setSize(float width, float height);

    // Overload for convenience to accept sf::Vector2f
    void setSize(const sf::Vector2f& size);

    // Sets the center of the camera's view, to follow the player
    void setCenter(const sf::Vector2f& center);

    // Returns the view for the window to use for drawing
    const sf::View& getView() const;

    // Prevents the camera from showing areas outside the map boundaries
    void clampToArea(const sf::FloatRect& worldBounds);

private:
    sf::View m_view;
};