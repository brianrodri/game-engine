#pragma once
#include <SFML/Graphics.hpp>

enum class Actions { NONE, UPDATE, RENDER, BOTH };

struct BaseComponent {};

template <Actions> struct Component;

template<>
struct Component<Actions::NONE> : BaseComponent {
    static const Actions action_value = Actions::NONE; 
};

template<>
struct Component<Actions::UPDATE> : BaseComponent {
    static const Actions action_value = Actions::UPDATE; 
    virtual void update(float dt) = 0;
};

template<>
struct Component<Actions::RENDER> : BaseComponent {
    static const Actions action_value = Actions::RENDER; 
    virtual void draw(sf::RenderTarget&, sf::RenderStates) const = 0;
};

template<>
struct Component<Actions::BOTH> : BaseComponent {
    static const Actions action_value = Actions::BOTH; 
    virtual void update(float dt) = 0;
    virtual void draw(sf::RenderTarget&, sf::RenderStates) const = 0;
};

using PODComponent = Component<Actions::NONE>;
using ProcessorComponent = Component<Actions::UPDATE>;
using PainterComponent = Component<Actions::RENDER>;
using Component = Component<Actions::BOTH>;
