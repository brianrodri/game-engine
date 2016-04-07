#pragma once
#include "Component.h"
#include <SFML/System/Vector2.hpp>

struct VelocityComp : Component {

    //! Explicit constructor w/ coordinates
    VelocityComp(float, float);
    VelocityComp(const sf::Vector2f&);

    sf::Vector2f value;

} /*struct VelocityComp*/;
