#include "PhysicsComps.h"


PositionComp::PositionComp(float x, float y) : value{x, y}
{
}

PositionComp::PositionComp(const sf::Vector2f& src) : value{src}
{
}

VelocityComp::VelocityComp(float x, float y) : value{x, y}
{
}

VelocityComp::VelocityComp(const sf::Vector2f& src) : value{src}
{
}

AccelerationComp::AccelerationComp(float x, float y) : value{x, y}
{
}

AccelerationComp::AccelerationComp(const sf::Vector2f& src) : value{src}
{
}

SimpleMotionComp::SimpleMotionComp(PositionComp& p_, VelocityComp& v_) : pos{p_}, vel{v_}
{
}

void SimpleMotionComp::update(float dt)
{
    pos.value += (vel.value * dt);
}

