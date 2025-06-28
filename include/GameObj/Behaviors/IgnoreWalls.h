#pragma once
#include "GameObj/Behaviors/CollisionBehavior.h"
#include "GameObj/MovingObj/Enemy.h"

class IgnoreWalls : public CollisionBehavior
{
public:
	void handleCollision(GameObject* other)
	{
		if (other->getType() == ObjectType::WALL)
		{
			// Ignore collision with walls
			return;
		} 
		
		// Handle other collisions normally
		if (m_owner)
		{
			m_owner->handleCollision(other);
		}
	}
};