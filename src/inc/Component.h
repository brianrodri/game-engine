#pragma once
#include <SFML/Graphics.hpp>

//! The Component structure
/**
 * I feel components are the most efficient way to stay sane when trying to
 * create a game A.S.A.P. that is: testable, composable, and as _simple_ as
 * possible.
 *
 * I facilitate this by splitting the components into categories that allow me
 * to classify the types of actions that a game needs done, and allow them to
 * achieve maximum efficiency through fine-tuning each of said categories.
 *
 * To maximize the use in these Components, try to make them depend on each
 * other such that they are as simplistic as possible.  Components are given
 * a reference to others that will live 
 *
 * Each category has its own benefits and quirks, which can be found under
 * their documentation.
 */
struct BaseComponent { };


//! `PODComponent` holds data and does absolutely nothing else.
/**
 * These components are constructed using a special allocator which works on a
 * stack array of a reasonable size.  Any excess memory is constructed on the
 * heap, so they will be constructible as any regular C++ data type would.
 *
 * Some good ideas for PODComponents:
 *    - NPC Text Data
 *    - Shop Info
 *    - Level Maps
 *
 * HINT: Use static storage wisely!
 */
struct PODComponent : BaseComponent {
};


//! `ProcessorComponent` performs computations and does absolutely nothing else.
/**
 * These components are well suited to threading, and should be given extra weight
 * in thread-balancing algorithms.
 *
 * Some good ideas for ProcessorComponents:
 *    - Random-Encounter Controller
 *    - Key-Combination Listener
 *    - Auto-Saver
 */
struct ProcessorComponent : BaseComponent {
    virtual void update(float dt) = 0;
};


//! `PainterComponent` renders a scene and does aboslutely nothing else.
/**
 * These components have more favorable priorities over regular components in
 * the render step of the engine cycle.
 *
 * Some good ideas for PainterComponents:
 *    - Menu screens
 *    - Credits
 *    - HUD Display
 */
struct PainterComponent : BaseComponent {
    virtual void draw(sf::RenderTarget&, sf::RenderStates) const = 0;
};


//! An updating, rendering, self-managing, and composable "action" encapsulation.
/**
 * Everything else!  Prefer to create components that capture some kind of
 * action, or perhaps something that can be intuitively acted upon.
 */
struct Component : BaseComponent {
    virtual void update(float dt) = 0;
    virtual void draw(sf::RenderTarget&, sf::RenderStates) const = 0;
};
