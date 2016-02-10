#ifndef HEADER_GAME_ENGINE_SRC_INC_COMPONENTS_VELOCITY_H_INCLUDED
#define HEADER_GAME_ENGINE_SRC_INC_COMPONENTS_VELOCITY_H_INCLUDED
#include "Component.h"

struct VelocityC : BaseComponent {
    sf::Vector2f value;

    VelocityC() = default;
    VelocityC(const VelocityC&) = default;
    VelocityC(VelocityC&&) = default;
    VelocityC& operator=(const VelocityC&) = default;
    VelocityC& operator=(VelocityC&&) = default;

    //! Explicit constructor w/ coordinates
    VelocityC(float x, float y) : value{x, y}
    {
    }

    //! Explicit constructor w/ pre-made vector
    VelocityC(const sf::Vector2f& vec) : value{vec}
    {
    }
} /*struct VelocityC*/;

#endif
