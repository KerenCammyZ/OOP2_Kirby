// Wall.cpp
#include "GameObj/FixedObj/Wall.h"
#include "GameObj/MovingObj/Kirby.h"
#include "GameObjectFactory.h"
#include "GlobalSizes.h"
#include <iostream>

sf::Color wallColor(255, 0, 0);

static bool isFloorRegistered = GameObjectFactory::instance().registerType(
	sf::Color(wallColor), // The color key for a Wall object
	[](sf::Vector2f position) -> std::unique_ptr<GameObject> // The blueprint
	{
		// This lambda function creates and returns a new wall object
		// when the factory is asked to create one for the color red.
		auto wall = std::make_unique<Wall>();
		// Note: position is passed to the factory create function but not used here
		// because Wall's constructor doesn't need it. The position is set later.
		return wall;
	}
);

void Wall::handleCollision(Kirby* kirby)
{
	//kirby->setPosition(kirby->getOldPosition());
	kirby->handleCollision(this);
}

void Wall::draw(sf::RenderTarget& target) const
{
	;
}

void Wall::setSize(const sf::Vector2f& size)
{
	GameObject::setSize(size);
	m_collisionShape.setSize(size);
	//m_collisionShape.setFillColor(sf::Color::Red); // Make it invisible
	//m_collisionShape.setOrigin(size.x / 2.f, size.y / 2.f);
	//m_debugShape.setSize(size);
	//m_debugShape.setOrigin(size.x / 2.f, size.y / 2.f); // Keep origin centered
}

void Wall::setPosition(const sf::Vector2f& position)
{
	GameObject::setPosition(position);
	m_collisionShape.setPosition(position);
	//m_debugShape.setPosition(position);
}

//#include "GameObj/FixedObj/Wall.h"
//#include "GameObj/MovingObj/Kirby.h"
//#include "GameObjectFactory.h"
//#include "GlobalSizes.h"
//#include <cmath>
//
//// Static lambda to register the Wall object type with the factory
//static bool isWallRegistered = GameObjectFactory::instance().registerType(
//	WALL, // The color key from GlobalSizes.h
//	[](sf::Vector2f position) -> std::unique_ptr<GameObject>
//	{
//		return std::make_unique<Wall>();
//	}
//);
//
//void Wall::handleCollision(Kirby* kirby)
//{
//	sf::FloatRect kirbyBounds = kirby->getBounds();
//	sf::FloatRect wallBounds = getBounds();
//	sf::Vector2f kirbyPrevPos = kirby->getOldPosition();
//	sf::FloatRect kirbyPrevBounds(
//		kirbyPrevPos - sf::Vector2f(kirbyBounds.width / 2.f, kirbyBounds.height / 2.f),
//		kirbyBounds.getSize()
//	);
//
//	// --- HORIZONTAL COLLISION CHECK (For Ramps and Walls) ---
//	// Check if we hit the wall from the side in the previous frame
//	bool hitFromLeft = kirbyPrevBounds.left + kirbyPrevBounds.width <= wallBounds.left;
//	bool hitFromRight = kirbyPrevBounds.left >= wallBounds.left + wallBounds.width;
//
//	if (hitFromLeft || hitFromRight)
//	{
//		// This is a horizontal collision. Now check if it's a small step or a real wall.
//		float kirbyFeetY = kirby->getPosition().y + kirbyBounds.height / 2.f;
//		float wallTopY = wallBounds.top;
//
//		if (wallTopY > kirbyFeetY - MAX_STEP_HEIGHT && wallTopY < kirbyFeetY)
//		{
//			// --- IT'S A RAMP/STEP ---
//			// The top of the wall is within our "step up" range.
//			// Treat it like a floor and pop Kirby up.
//			kirby->setGrounded(true);
//			kirby->setVelocity({ kirby->getVelocity().x, 0.f });
//			kirby->setPosition({ kirby->getPosition().x, wallBounds.top - kirbyBounds.height / 2.f });
//			return; // Collision handled
//		}
//		else
//		{
//			// --- IT'S A TALL WALL ---
//			// The ledge is too high to step up, so stop horizontal movement.
//			kirby->setVelocity({ 0.f, kirby->getVelocity().y });
//			kirby->setPosition({ kirbyPrevPos.x, kirby->getPosition().y });
//			return; // Collision handled
//		}
//	}
//
//	// --- VERTICAL COLLISION CHECK (For Ceilings and Floors) ---
//	// Check if we landed on top (same as floor logic)
//	if (kirbyPrevBounds.top + kirbyPrevBounds.height <= wallBounds.top)
//	{
//		kirby->setGrounded(true);
//		kirby->setVelocity({ kirby->getVelocity().x, 0.f });
//		kirby->setPosition({ kirby->getPosition().x, wallBounds.top - kirbyBounds.height / 2.f });
//	}
//	// Check if we hit our head on the bottom
//	else if (kirbyPrevBounds.top >= wallBounds.top + wallBounds.height)
//	{
//		kirby->setVelocity({ kirby->getVelocity().x, 0.f });
//		kirby->setPosition({ kirby->getPosition().x, wallBounds.top + wallBounds.height + kirbyBounds.height / 2.f });
//	}
//}

//#include "GameObj/FixedObj/Wall.h"
//#include "GameObj/MovingObj/Kirby.h"
//#include "GameObjectFactory.h"
//#include "GlobalSizes.h"
//#include <cmath>
//
//// Make sure you have this in GlobalSizes.h and that it's a bit larger than ENTITY_SIZE
//// const float MAX_STEP_HEIGHT = 25.0f;
//
//// Static lambda to register the Wall object type with the factory
//static bool isWallRegistered = GameObjectFactory::instance().registerType(
//	WALL,
//	[](sf::Vector2f position) -> std::unique_ptr<GameObject>
//	{
//		return std::make_unique<Wall>();
//	}
//);
//
//
//void Wall::handleCollision(Kirby* kirby)
//{
//	sf::FloatRect kirbyBounds = kirby->getBounds();
//	sf::FloatRect wallBounds = getBounds();
//	sf::Vector2f kirbyPrevPos = kirby->getOldPosition();
//	sf::FloatRect kirbyPrevBounds(
//		kirbyPrevPos - sf::Vector2f(kirbyBounds.width / 2.f, kirbyBounds.height / 2.f),
//		kirbyBounds.getSize()
//	);
//
//	// --- 1. RAMP/STEP-UP LOGIC (HIGHEST PRIORITY) ---
//	// First, determine if Kirby was moving into the wall horizontally from the side.
//	bool hitFromLeft = kirbyPrevBounds.left + kirbyPrevBounds.width <= wallBounds.left + 1.f; // add tolerance
//	bool hitFromRight = kirbyPrevBounds.left >= wallBounds.left + wallBounds.width - 1.f; // add tolerance
//
//	// Next, determine if the top of the wall qualifies as a "step".
//	float kirbyFeetY = kirbyPrevBounds.top + kirbyPrevBounds.height; // Kirby's feet in the last frame.
//	float wallTopY = wallBounds.top;
//	// A "step" is a ledge whose top is slightly above Kirby's previous foot position,
//	// but not higher than he can step up.
//	bool isAValidStep = (kirbyFeetY >= wallTopY) && (kirbyFeetY - wallTopY < MAX_STEP_HEIGHT);
//
//	// CONDITION: If Kirby hit the side of a wall AND that wall is a valid step...
//	if ((hitFromLeft || hitFromRight) && isAValidStep)
//	{
//		// ...then treat it as a ramp and pop Kirby on top.
//		kirby->setGrounded(true);
//		kirby->setVelocity({ kirby->getVelocity().x, 0.f });
//		kirby->setPosition({ kirby->getPosition().x, wallBounds.top - kirbyBounds.height / 2.f });
//	}
//	// --- 2. STANDARD COLLISION (FALLBACK) ---
//	// If it wasn't a step, proceed with normal collision checks.
//	else if (kirbyPrevBounds.top + kirbyPrevBounds.height <= wallBounds.top + 1.f) // Land on top
//	{
//		kirby->setGrounded(true);
//		kirby->setVelocity({ kirby->getVelocity().x, 0.f });
//		kirby->setPosition({ kirby->getPosition().x, wallBounds.top - kirbyBounds.height / 2.f });
//	}
//	else if (kirbyPrevBounds.top >= wallBounds.top + wallBounds.height - 1.f) // Hit head on bottom
//	{
//		kirby->setVelocity({ kirby->getVelocity().x, 0.f });
//		kirby->setPosition({ kirby->getPosition().x, wallBounds.top + wallBounds.height + kirbyBounds.height / 2.f });
//	}
//	else if (hitFromLeft) // Hit a tall wall from the left
//	{
//		kirby->setVelocity({ 0.f, kirby->getVelocity().y });
//		kirby->setPosition({ wallBounds.left - kirbyBounds.width / 2.f, kirby->getPosition().y });
//	}
//	else if (hitFromRight) // Hit a tall wall from the right
//	{
//		kirby->setVelocity({ 0.f, kirby->getVelocity().y });
//		kirby->setPosition({ wallBounds.left + wallBounds.width + kirbyBounds.width / 2.f, kirby->getPosition().y });
//	}
//}
