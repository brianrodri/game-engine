#pragma once
#include <experimental/type_traits>
#include "FactoryTuple.h"
#include "ComponentVisitors.h"
//#include <hana/int_c.h>
//#include <hana/hof/for_each.h>
#include <boost/hana.hpp>
#include <boost/hana/ext/std/tuple.hpp>
#include "Component.h"

/**
 * `Designed to compose several _related_ components as one functioning component. ComponentTuple` is built upon
 * `FactoryTuple`, which takes a series of **functions** as arguments
 */
template<typename... C>
class ComponentTuple : public Component {

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
    template<typename... F>
    constexpr ComponentTuple(F&&... f) : m_componentTuple{std::forward<F>(f)...}
    {
    }

    //! Access through compile time indices
    template<typename Idx>
    auto& operator[](Idx&& i)
    {
        return m_componentTuple[std::forward<Idx>(i)];
    }

    //! Access through compile time indices
    template<typename Idx>
    const auto& operator[](Idx&& i) const
    {
        return m_componentTuple[std::forward<Idx>(i)];
    }

    void update(float dt)
    {
        UpdateVisitor updater{dt};
        boost::hana::for_each(m_componentTuple.tie(), updater);
    }

    void draw(sf::RenderTarget& tar, sf::RenderStates stt) const
    {
        DrawVisitor drawer{tar, stt};
        boost::hana::for_each(m_componentTuple.ctie(), drawer);
    }

    constexpr auto tie()
    {
        return m_componentTuple.tie();
    }

    constexpr auto ctie() const
    {
        return m_componentTuple.ctie();
    }

private:

    FactoryTuple<C...>  m_componentTuple;

} /*class ComponentTuple*/;
