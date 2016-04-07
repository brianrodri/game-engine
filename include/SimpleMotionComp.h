#pragma once
#include "Component.h"

// Forward Declarations
struct PositionComp;
struct VelocityComp;

class SimpleMotionComp : Component {
public:
    SimpleMotionComp(PositionComp&, VelocityComp&);
    void update(float);

private:
    PositionComp& pos;
    VelocityComp& vel;

} /*struct SimpleMotionComp*/;
