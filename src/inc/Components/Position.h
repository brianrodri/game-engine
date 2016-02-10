#ifndef HEADER_GAME_ENGINE_SRC_INC_COMPONENTS_POSITION_H_INCLUDED
#define HEADER_GAME_ENGINE_SRC_INC_COMPONENTS_POSITION_H_INCLUDED
#include "Component.h"

struct PositionC : BaseComponent {
    sf::Vector2f value;

    PositionC() = default;
    PositionC(const PositionC&) = default;
    PositionC(PositionC&&) = default;
    PositionC& operator=(const PositionC&) = default;
    PositionC& operator=(PositionC&&) = default;

    //! Explicit constructor w/ coordinates
    PositionC(float x, float y) : value{x, y}
    {
    }

    //! Explicit constructor w/ pre-made vector
    PositionC(const sf::Vector2f& vec) : value{vec}
    {
    }
} /*struct PositionC*/;

#endif
