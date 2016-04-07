#include "SimpleMotionComp.h"
#include "PositionComp.h"
#include "VelocityComp.h"


SimpleMotionComp::SimpleMotionComp(PositionComp& p_, VelocityComp& v_) : pos{p_}, vel{v_}
{
}

void SimpleMotionComp::update(float dt)
{
    pos.value += (vel.value * dt);
}
