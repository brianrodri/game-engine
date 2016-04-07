#pragma once
#include "Component.h"
#include <SFML/System/Vector2.hpp>

struct AccelComp : Component {

    //! Explicit through components
    AccelComp(float x, float y);

    //! Constructs directly from a 2D vector value
    AccelComp(const sf::Vector2f& vec);

    //! Member value
    sf::Vector2f value;

} /*struct AccelComp*/;
