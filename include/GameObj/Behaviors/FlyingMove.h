#include "GameObj/Behaviors/MoveBehavior.h"
#include "GameObj/MovingObj/Enemy.h"

class FlyingMove : public MoveBehavior
{
public:
	void move(float deltaTime)
	{
		if (!m_owner) return;

		sf::Vector2f position = m_owner->getPosition();
		sf::Vector2f direction = m_owner->getDirection();
		float speed = m_owner->getSpeed();
		
		if(position.y < VIEW_HEIGHT/10 || position.y > VIEW_HEIGHT/3)
		{
			// Reverse direction if we hit the top or bottom of the screen
			//direction.y = -direction.y;
			//m_owner->setDirection(direction);
		}

		m_owner->setPosition(
			sf::Vector2f(position.x + direction.x * speed * deltaTime,
				         position.y + direction.y * speed * deltaTime));
	}
};