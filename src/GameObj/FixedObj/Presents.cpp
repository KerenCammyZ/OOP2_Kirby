#include "GameObj/FixedObj/Present.h"

void Present::handleCollision(Kirby* kirby)
{
	if (!m_isCollected)
	{
		applyEffect(kirby);
		m_isCollected = true;
	}
}