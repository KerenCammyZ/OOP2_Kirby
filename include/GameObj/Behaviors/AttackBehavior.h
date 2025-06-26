#pragma once
class Enemy;

class AttackBehavior
{
public:
	virtual ~AttackBehavior() = default;
	virtual void attack(float deltaTime) = 0;
};