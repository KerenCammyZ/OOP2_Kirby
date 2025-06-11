#pragma once
#include <SFML/Graphics.hpp>
// Disable MSVC analysis warnings for the box2d include
#pragma warning(push)
#pragma warning(disable: 26495 26813)
#include "box2d/box2d.h"
#pragma warning(pop)

#include "Renderer.h"
#include "GlobalSizes.h"

class GameObject
{
public:
	// constructor and destructor
	GameObject();
	virtual ~GameObject();

	// virtual methods
	virtual void update(float dt) = 0;
	virtual void draw(Renderer& renderer) const;
	virtual void onCollision(GameObject* other) {}

	void draw(sf::RenderTarget& target) const;
	/*void initPhysics(b2World& world, const b2Vec2& positionMeters,
		bool fixedRotation = true, float density = 1.0f, float friction = 0.3f);*/
	void initPhysics(b2World& world, const sf::Vector2f& positionPixels, 
		bool fixedRotation = true, float density = 1.0f, float friction = 0.3f);

	// setters and getters
	void setPosition(const sf::Vector2f& position);
	sf::Vector2f getPosition() const;

	void setSize(const sf::Vector2f& size);;
	sf::Vector2f getSize() const;

	void setTexture(std::shared_ptr<sf::Texture> texture);
	sf::FloatRect getBounds() const;

protected:
	sf::Vector2f m_position;
	sf::Vector2f m_size;
	std::shared_ptr<sf::Texture> m_texture; // Use shared_ptr for texture management
	sf::Sprite m_sprite; // Sprite for rendering the texture
	b2Body* m_body = nullptr; // Pointer to the Box2D body associated with this GameObject

private:
	void createPhysicsBody(b2World& world, const b2Vec2& positionMeters,
		bool fixedRotation, float density, float friction);
};