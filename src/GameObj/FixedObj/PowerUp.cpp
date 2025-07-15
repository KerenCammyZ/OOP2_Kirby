// PowerUp.cpp
#include "GameObj/FixedObj/PowerUp.h"

void PowerUp::handleCollision(Kirby* kirby)
{
	if (!m_isCollected)
	{
		applyEffect(kirby);
		m_isCollected = true;
	}
}