// Water.cpp
#include "GameObj/FixedObj/Water.h"
#include "GameObj/MovingObj/Kirby.h"
#include "GameObjectFactory.h"
#include <SFML/Graphics.hpp>
#include <iostream>

const sf::Color waterColor(0, 0, 200);

void Water::handleCollision(Kirby* kirby)
{
	kirby->setInWater(true);
}