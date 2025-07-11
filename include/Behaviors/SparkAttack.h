#pragma once
#include "Behaviors/AttackBehavior.h"

class SparkAttack : public AttackBehavior
{
public:
    SparkAttack();
    void attack(float deltaTime) override;

private:
    float m_flashTimer;
};