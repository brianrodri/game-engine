#ifndef HEADER_GAME_ENGINE_SRC_INC_COMPONENTS_POSITION_H_INCLUDED
#define HEADER_GAME_ENGINE_SRC_INC_COMPONENTS_POSITION_H_INCLUDED
#include "Component.h"

struct PositionC : BaseComponent {
    sf::Vector2f value;

    PositionC(float x, float y) : value{x, y}
    {
    }

    PositionC(sf::Vector2f vec) : value{vec}
    {
    }
} /*struct PositionC*/;

#endif
