#include "Behaviors/MoveBehavior.h"
#include "GameObj/MovingObj/Enemy.h"

class PatrolMove : public MoveBehavior
{
public:
	void move(float deltaTime)
	{
		if (!m_owner) return;

		sf::Vector2f position = m_owner->getPosition();
		float direction = m_owner->getDirection().x;
		float speed = m_owner->getSpeed();
		m_owner->setPosition(
			sf::Vector2f(position.x + direction * speed * deltaTime, position.y));
	}
};