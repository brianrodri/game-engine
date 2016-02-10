#ifndef HEADER_GAME_ENGINE_SRC_INC_COMPONENTS_SIMPLE_MOTION_H_INCLUDED
#define HEADER_GAME_ENGINE_SRC_INC_COMPONENTS_SIMPLE_MOTION_H_INCLUDED
#include "Component.h"
#include "Position.h"
#include "Velocity.h"

class SimpleMotionC : ProcessorComponent {
public:
    SimpleMotionC(PositionC& pos_, VelocityC& vel_) : pos{pos_}, vel{vel_} {};

    virtual void update(float dt) override
    {
        pos.value += dt * vel.value;
    }

private:
    PositionC& pos;
    VelocityC& vel;
} /*struct SimpleMotionC*/;

#endif
