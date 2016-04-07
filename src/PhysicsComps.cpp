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

MotionComp::MotionComp(
    gsl::not_null<PositionComp*> posPtr
  , gsl::not_null<VelocityComp*> velPtr
  , AccelerationComp* accPtr
    )
  : pos{posPtr}
  , vel{velPtr}
  , acc{accPtr}
{
}

void MotionComp::update(float dt)
{
    if (acc) { vel->value += (acc->value * dt); }
    pos->value += (vel->value * dt);
}

