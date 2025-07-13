#include "States/KirbyStates/KirbyJumpingState.h"
#include "GameObj/MovingObj/Kirby.h"
#include "GlobalSizes.h";

void KirbyJumpingState::enter(Kirby& kirby)
{
	// Apply an immediate upward velocity.
	kirby.setVelocity({ kirby.getVelocity().x, -JUMP_STRENGTH });
	kirby.setGrounded(false);

	kirby.setAnimation("jumping");
}