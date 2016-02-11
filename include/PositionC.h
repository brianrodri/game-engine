#pragma once
#include "Component.h"
#include <SFML/System/Vector2.hpp>

struct PositionC : PODComponent {
    sf::Vector2f value;

    //! Explicit construction
    PositionC(float x, float y);
    PositionC(const sf::Vector2f& vec);
    //! Destructor behavior
    virtual ~PositionC() = default;
    //! Default behavior
    PositionC() = default;
    PositionC(const PositionC&) = default;
    PositionC(PositionC&&) = default;
    PositionC& operator=(const PositionC&) = default;
    PositionC& operator=(PositionC&&) = default;

} /*struct PositionC*/;
