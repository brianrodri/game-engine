#pragma once
#include <experimental/type_traits>
#include "Component.h"
#include "DrawVisitor.h"
#include "FactoryTuple.h"
#include "UpdateVisitor.h"
#include <aetee/int_c.h>
#include <aetee/hof/for_each.h>

/**
 * `Designed to compose several _related_ components as one functioning component. ComponentTuple` is built upon
 * `FactoryTuple`, which takes a series of **functions** as arguments
 */
template <typename... C>
class ComponentTuple : Component {
    static_assert(std::conjunction<std::is_base_of<Component, C>...>::value);

public:

    //! Default behavior
    ComponentTuple() = default;
    virtual ~ComponentTuple() = default;

    //! Can not be moved in any way due to possible complexities
    ComponentTuple(const ComponentTuple&) = delete;
    ComponentTuple(ComponentTuple&&) = delete;
    ComponentTuple& operator=(const ComponentTuple&) = delete;
    ComponentTuple& operator=(ComponentTuple&&) = delete;

    //! Explicit construction w/ factories
    template <typename... F>
    constexpr ComponentTuple(F&&... f) : m_componentTuple{std::forward<F>(f)...}
    {
    }

    //! Access through compile time indices
    template <size_t I>
    auto& operator[](aetee::idx_constant_t<I> i)
    {
        return m_componentTuple[i];
    }

    //! Access through compile time indices
    template <size_t I>
    const auto& operator[](aetee::idx_constant_t<I> i) const
    {
        return m_componentTuple[i];
    }

    template <typename K>
    auto& operator[](aetee::type_constant_t<K> k)
    {
        return m_componentTuple[k];
    }

    template <typename K>
    const auto& operator[](aetee::type_constant_t<K> k) const
    {
        return m_componentTuple[k];
    }

    void update(float dt)
    {
        aetee::for_each(m_componentTuple, UpdateVisitor{dt});
    }

    void draw(sf::RenderTarget& tar, sf::RenderStates stt) const
    {
        aetee::for_each(m_componentTuple, DrawVisitor{tar, stt});
    }

private:

    FactoryTuple<C...>  m_componentTuple;

} /*class ComponentTuple*/;
