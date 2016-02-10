#ifndef HEADER_GAME_ENGINE_SRC_INC_COMPONENTS_VELOCITY_H_INCLUDED
#define HEADER_GAME_ENGINE_SRC_INC_COMPONENTS_VELOCITY_H_INCLUDED
#include "Component.h"

struct VelocityC : BaseComponent {
    sf::Vector2f value;

    VelocityC(float x, float y) : value{x, y}
    {
    }

    VelocityC(sf::Vector2f vec) : value{vec}
    {
    }
} /*struct VelocityC*/;

#endif
