#pragma once
#include "States/KirbyAirborneState.h"

// The falling state has no unique logic of its own. It's just an
// airborne state that wasn't initiated by a jump.
class KirbyFallingState : public KirbyAirborneState {};