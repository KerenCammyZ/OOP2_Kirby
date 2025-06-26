#pragma once
class Enemy;

class MoveBehavior
{
public:
	virtual ~MoveBehavior() = default;
	virtual void move(float deltaTime) = 0;
	
private:
};