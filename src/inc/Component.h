#pragma once
#include <SFML/Graphics.hpp>

struct BaseComponent {};

//! A `Component` dedicated to holding data and absolutely nothing else.
/**
 * A special allocator is used for these POD types to help take advantage of
 * their versatility as `memcpy`-able objects.
 */
struct PODComponent : BaseComponent {
};

//! A `Component` dedicated to computation and absolutely nothing else.
/**
 * These components are well suited to threading, and should be given extra weight
 * in thread balancing algorithms.
 */
struct ProcessorComponent : BaseComponent {
    virtual void update(float dt) = 0;
};

//! A `Component` dedicated to rendering a scene and aboslutely nothing else.
/**
 * These components have higher priority than regular components in the render
 * step of the engine cycle.
 */
struct PainterComponent : BaseComponent {
    virtual void draw(sf::RenderTarget&, sf::RenderStates) const = 0;
};

//! An updating, rendering, self-managing, and composable "action" encapsulation.
/**
 *
 */
struct Component : BaseComponent {
    virtual void update(float dt) = 0;
    virtual void draw(sf::RenderTarget&, sf::RenderStates) const = 0;
};
