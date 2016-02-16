#pragma once
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <memory>
#include <aetee/type_c.h>

// Forward Declarations
struct BaseComponent;
struct PODComponent;
struct ProcessorComponent;
struct PainterComponent;
struct Component;



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
struct BaseComponent {
    virtual ~BaseComponent() = default;
} /*struct BaseComponent*/;


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
    virtual ~PODComponent() = default;
} /*struct PODComponent*/;


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
    virtual ~ProcessorComponent() = default;
    virtual void update(float dt) = 0;
} /*struct ProcessorComponent*/;


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
    virtual ~PainterComponent() = default;
    virtual void draw(sf::RenderTarget&, sf::RenderStates) const = 0;
} /*struct PainterComponent*/;


//! An updating, rendering, self-managing, and composable "action" encapsulation.
/**
 * Everything else!  Prefer to create components that capture some kind of
 * action, or perhaps something that can be intuitively acted upon.
 */
struct Component : ProcessorComponent, PainterComponent {
    virtual ~Component() = default;
} /*struct Component*/;

//! Helper class that determines whether a Component may be updated
struct ComponentUpdateChecker {
    template <typename C> constexpr auto operator()(aetee::type_constant_t<C>) const
    {
        return std::disjunction<std::is_base_of<ProcessorComponent, C>>{};
    }

    ProcessorComponent* operator()(BaseComponent* ptr) const
    {
        return dynamic_cast<ProcessorComponent*>(ptr);
    }
} /*struct updateChecker*/;

//! Helper class that determines whether a Component may be rendered
struct ComponentRenderChecker {
    template <typename C> constexpr auto operator()(aetee::type_constant_t<C>) const
    {
        return std::disjunction<std::is_base_of<PainterComponent, C>>{};
    }

    PainterComponent* operator()(BaseComponent* ptr) const
    {
        return dynamic_cast<PainterComponent*>(ptr);
    }
} /*struct renderChecker*/;
