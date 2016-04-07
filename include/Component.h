#pragma once
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <memory>

struct Component {
    virtual ~Component() = default;
};

using ComponentPtr = std::unique_ptr<Component>;
