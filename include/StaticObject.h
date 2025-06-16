// StaticObject.h : Static Game Objects. Inherits from class GameObject
#pragma once
#include "GameObject.h"

class StaticObject : public GameObject
{
public:
	StaticObject();
	~StaticObject() = default;
	// Static objects don't move, so their update is empty,
	// but it must be implemented.
	void update(float dt) override {}; // TODO consider declaring first in MovingObject
	                                   // otherwise explain why it is declared in GameObject

	// By default, static objects don't react to generic collisions
	void handleCollision(GameObject* other) override;
	virtual void handleCollision(Kirby* kirby) = 0;

	// Override drawing and property setters to handle the debug shape
	void draw(sf::RenderTarget& target) const override;
	void setSize(const sf::Vector2f& size) override;
	void setPosition(const sf::Vector2f& position) override;

protected:
	sf::RectangleShape m_debugShape;
};

