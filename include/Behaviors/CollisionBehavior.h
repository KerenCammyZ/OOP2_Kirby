#pragma once
#include "GameObj/GameObject.h"
class Enemy;

class CollisionBehavior
{
public:
	virtual ~CollisionBehavior() = default;
	virtual void handleCollision(GameObject* other) = 0;
	void setOwner(Enemy* owner) { m_owner = owner; }

protected:
	Enemy* m_owner = nullptr;
};