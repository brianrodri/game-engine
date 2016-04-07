#pragma once
#include "Component.h"
#include <SFML/System/Vector2.hpp>

struct AccelComp : Component {

    //! Explicit construction
    AccelComp(float x, float y);
    AccelComp(const sf::Vector2f& vec);

    //! Member value
    sf::Vector2f value;

} /*struct AccelComp*/;
