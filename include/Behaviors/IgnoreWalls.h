#pragma once
#include "Behaviors/CollisionBehavior.h"
#include "GameObj/MovingObj/Enemy.h"

class IgnoreWalls : public CollisionBehavior
{
public:
	void handleCollision(GameObject* other)
	{
		// entering this block returns to calling function immediately
		if (other->getType() == ObjectType::WALL)
		{
			return; // Ignore collision with walls
		} 
	
		// ObjectType is not a Wall
		if (m_owner)
		{
			m_owner->handleCollision(other);
		}
	}
};