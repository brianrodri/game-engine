#pragma once
#include "Component.h"
#include <SFML/System/Vector2.hpp>

struct PositionComp : Component {

    //! Explicit construction
    PositionComp(float x, float y);
    PositionComp(const sf::Vector2f& vec);

    sf::Vector2f value;

} /*struct PositionComp*/;

struct VelocityComp : Component {

    //! Explicit constructor w/ coordinates
    VelocityComp(float, float);
    VelocityComp(const sf::Vector2f&);

    sf::Vector2f value;

} /*struct VelocityComp*/;

struct AccelerationComp : Component {

    //! Explicit constructor w/ coordinates
    AccelerationComp(float, float);
    AccelerationComp(const sf::Vector2f&);

    sf::Vector2f value;

} /*struct VelocityComp*/;

class SimpleMotionComp : Component {
public:

    SimpleMotionComp(PositionComp&, VelocityComp&);
    void update(float);

private:

    PositionComp& pos;
    VelocityComp& vel;

} /*struct SimpleMotionComp*/;

