// FixedObject.h
#pragma once
#include "GameObj/GameObject.h"

class FixedObject : public GameObject
{
public:
	FixedObject();
	~FixedObject() = default;
	void update(float dt) override {};

	virtual void handleCollision(Kirby* kirby) = 0;
	void handleCollision(GameObject* other) override;
	//void handleCollision(Door* door) override = 0;
	void handleCollision(Enemy* enemy) override;

	// --- OVERRIDE DECLARATIONS ---
	void draw(sf::RenderTarget& target) const override;
	void setSize(const sf::Vector2f& size) override;
	void setPosition(const sf::Vector2f& position) override;
	sf::FloatRect getBounds() const override;

protected:
	sf::RectangleShape m_collisionShape;
};
