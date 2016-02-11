#include "SimpleMotionC.h"
#include "PositionC.h"
#include "VelocityC.h"


SimpleMotionC::SimpleMotionC(PositionC& p_, VelocityC& v_) : pos{p_}, vel{v_}
{
}

void SimpleMotionC::update(float dt)
{
    pos.value += (vel.value * dt);
}
