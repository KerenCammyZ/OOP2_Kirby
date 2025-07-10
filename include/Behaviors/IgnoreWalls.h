#pragma once
#include "Behaviors/CollisionBehavior.h"
#include "GameObj/MovingObj/Enemy.h"

class IgnoreWalls : public CollisionBehavior
{
public:
	void handleCollision(GameObject* other)
	{
		if (other->getType() == ObjectType::WALL)
		{
			; // do nothing
		} 
	
		// ObjectType is not a Wall
		if (other->getType() == ObjectType::ENEMY) {
			m_owner->reverseDirection();
			m_owner->setPosition(m_owner->getOldPosition());
		}
	}
};