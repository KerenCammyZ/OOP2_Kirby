// Wall.h
#pragma once
#include "GameObj/FixedObj/FixedObject.h"

class Wall : public FixedObject  
{  
public:  
	Wall() = default;  

	void handleCollision(Kirby* kirby) override;  
	void handleCollision(Door* door) override {}; // GameObject Inheritance 
	void handleCollision(Enemy* enemy) override;

	void draw(sf::RenderTarget& target) const override;
	void setSize(const sf::Vector2f& size) override;
	void setPosition(const sf::Vector2f& position) override;

	ObjectType getType() const { return ObjectType::WALL; }

private:  
	static bool m_registeritem; // Static member to register this type with the factory
};