#pragma once
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <memory>

//! Base class for associating Components with each other
struct Component {
    virtual ~Component() = default;
};

//! Helper type for owning-pointers to Components
using ComponentPtr = std::unique_ptr<Component>;
