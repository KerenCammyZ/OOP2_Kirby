#include "MovingObject.h"

MovingObject::MovingObject() {}

MovingObject::~MovingObject() = default;


void MovingObject::update(float dt)
{
	move(dt);
}

