#include "GameObj/Behaviors/MoveBehavior.h"

class FlyingMove : public MoveBehavior
{
public:
	void move(float deltaTime)
	{
		std::cout << "FlyingMove" << std::endl;
	}
};