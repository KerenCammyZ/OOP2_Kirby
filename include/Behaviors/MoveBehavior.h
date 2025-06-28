#pragma once
class Enemy;

class MoveBehavior
{
public:
	virtual ~MoveBehavior() = default;
	virtual void move(float deltaTime) = 0;
	void setOwner(Enemy* owner) { m_owner = owner; }
	
protected:
	Enemy* m_owner = nullptr;
};