#pragma once
class Enemy;

class AttackBehavior
{
public:
	virtual ~AttackBehavior() = default;
	virtual void attack(float deltaTime) = 0;
	void setOwner(Enemy* owner) { m_owner = owner; }

protected:
	Enemy* m_owner = nullptr;
};