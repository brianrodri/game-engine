#pragma once
#include "Component.h"

// Forward Declarations
struct PositionC;
struct VelocityC;

class SimpleMotionC : ProcessorComponent {
public:
    SimpleMotionC(PositionC&, VelocityC&);
    virtual ~SimpleMotionC() = default;

    virtual void update(float) override;

private:
    PositionC& pos;
    VelocityC& vel;
} /*struct SimpleMotionC*/;
