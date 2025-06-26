#include "GameObj/Behaviors/MoveBehavior.h"
#include <iostream>

class PatrolMove : public MoveBehavior
{
public:
	void move(float deltaTime)
	{
		std::cout << "PatrolMove" << std::endl;
	}
};