// FixedObject.h
#pragma once
#include "GameObj/GameObject.h"

class FixedObject : public GameObject
{
public:
	FixedObject();
	~FixedObject() = default;
	void update(float dt) override {}; // TODO: maybe declare first in MovingObject

	// By default, static objects don't react to generic collisions
	void handleCollision(GameObject* other) override;
	virtual void handleCollision(Kirby* kirby) = 0;
	void handleCollision(Door* door) override = 0;

	// Override drawing and property setters to handle the debug shape
	void draw(sf::RenderTarget& target) const override;
	void setSize(const sf::Vector2f& size) override;
	void setPosition(const sf::Vector2f& position) override;

protected:
	sf::RectangleShape m_debugShape;
};

