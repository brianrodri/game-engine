#pragma once
#include "Component.h"
#include <SFML/System/Vector2.hpp>

struct VelocityC : PODComponent {
    sf::Vector2f value;

    //! Explicit constructor w/ coordinates
    VelocityC(float, float);
    VelocityC(const sf::Vector2f&);
    //! Destructor behavior
    virtual ~VelocityC() = default;
    //! Default behavior
    VelocityC() = default;
    VelocityC(const VelocityC&) = default;
    VelocityC(VelocityC&&) = default;
    VelocityC& operator=(const VelocityC&) = default;
    VelocityC& operator=(VelocityC&&) = default;
} /*struct VelocityC*/;
