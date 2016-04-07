#pragma once
#include "Component.h"
#include <SFML/System/Vector2.hpp>

struct PositionComp : Component {

    //! Explicit construction
    PositionComp(float x, float y);
    PositionComp(const sf::Vector2f& vec);

    sf::Vector2f value;

} /*struct PositionComp*/;
