#pragma once
#include "Component.h"
#include <type_traits>
#include <utility>

namespace detail {

template <typename C, typename = void>
class DrawVisitorResolverFunctor {

    void operator()(const C& visitee, sf::RenderTarget& target, sf::RenderStates states) const
    {
    }

};

template <typename C>
struct DrawVisitorResolverFunctor
  < C
  , decltype(std::declval<const C&>().draw(std::declval<sf::RenderTarget&>(), std::declval<sf::RenderStates>()))
    > {

    void operator()(const C& visitee, sf::RenderTarget& target, sf::RenderStates states) const
    {
        visitee.draw(target, states);
    }

};

} /*namespace detail*/;

class DrawVisitor {

public:

    DrawVisitor(sf::RenderTarget& targetIn, sf::RenderStates statesIn);

    template <typename C>
    void operator()(const C& visitee) const
    {
        static_assert(std::is_base_of<Component, C>::value);
        detail::DrawVisitorResolverFunctor<C>{}(visitee, m_target, m_states);
    }

private:

    std::reference_wrapper<sf::RenderTarget> m_target;
    sf::RenderStates m_states;

} /*class DrawVisitor*/;

