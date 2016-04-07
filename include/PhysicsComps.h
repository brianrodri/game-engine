#pragma once
#include <SFML/System/Vector2.hpp>

struct PositionComp {

    //! Explicit construction
    PositionComp(float x, float y);
    PositionComp(const sf::Vector2f& vec);

    sf::Vector2f value;

} /*struct PositionComp*/;

struct VelocityComp {

    //! Explicit constructor w/ coordinates
    VelocityComp(float, float);
    VelocityComp(const sf::Vector2f&);

    sf::Vector2f value;

} /*struct VelocityComp*/;

struct AccelerationComp {

    //! Explicit constructor w/ coordinates
    AccelerationComp(float, float);
    AccelerationComp(const sf::Vector2f&);

    sf::Vector2f value;

} /*struct VelocityComp*/;

class MotionComp {

public:
    MotionComp(gsl::not_null<PositionComp*>, gsl::not_null<VelocityComp*>, AccelerationComp* = nullptr);
    void update(float);

private:
    PositionComp*  pos;
    VelocityComp*  vel;
    AccelerationComp*  acc;

} /*class MotionComp*/;

