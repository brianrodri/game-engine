#pragma once
#include "Component.h"
#include <SFML/System/Vector2.hpp>

struct AccelerationC : PODComponent {
    sf::Vector2f value;

    //! Explicit construction
    AccelerationC(float x, float y);
    AccelerationC(const sf::Vector2f& vec);

    //! Destructor behavior
    virtual ~AccelerationC() = default;

    //! Default behavior
    AccelerationC() = default;
    AccelerationC(const AccelerationC&) = default;
    AccelerationC(AccelerationC&&) = default;
    AccelerationC& operator=(const AccelerationC&) = default;
    AccelerationC& operator=(AccelerationC&&) = default;

} /*struct AccelerationC*/;
