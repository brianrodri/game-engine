#pragma once
#include <functional>
#include <type_traits>
#include <utility>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

namespace detail {

template<typename C, typename = void>
class DrawVisitorResolverFunctor {

    void operator()(const C& visitee, sf::RenderTarget& target, sf::RenderStates states) const { }

};


template<typename C>
struct DrawVisitorResolverFunctor
  < C
  , decltype(std::declval<const C&>().draw(std::declval<sf::RenderTarget&>(), std::declval<sf::RenderStates>()))
    > {

    void operator()(const C& visitee, sf::RenderTarget& target, sf::RenderStates states) const
    {
        visitee.draw(target, states);
    }

};


//! Default behavior, do nothing
template<typename C, typename = void>
struct UpdateVisitorResolverFunctor {

    void operator()(C& visitee, float dt) { }

} /*struct UpdateVisitorResolverFunctor*/;


//! Specialized behavior, call update when it exists
template<typename C>
struct UpdateVisitorResolverFunctor
  < C
  , decltype(std::declval<C&>().update(std::declval<float>()))
    > {

    void operator()(C& visitee, float dt)
    {
        visitee.update(dt);
    }

} /*struct UpdateVisitorResolverFunctor*/;

} /*namespace detail*/;


class UpdateVisitor {

public:

    UpdateVisitor(float dtIn);

    template<typename C>
    void operator()(C& visitee)
    {
        detail::UpdateVisitorResolverFunctor<C>{}(visitee, m_dt);
    }

    template<typename C>
    void operator()(std::reference_wrapper<C> visitee)
    {
        detail::UpdateVisitorResolverFunctor<C>{}(visitee.get(), m_dt);
    }

private:

    float m_dt;

} /*struct UpdateVisitor*/;


class DrawVisitor {

public:

    DrawVisitor(sf::RenderTarget& targetIn, sf::RenderStates statesIn);

    template<typename C>
    void operator()(const C& visitee) const
    {
        detail::DrawVisitorResolverFunctor<C>{}(visitee, m_target.get(), m_states);
    }

    template<typename C>
    void operator()(std::reference_wrapper<const C> visitee) const
    {
        detail::DrawVisitorResolverFunctor<C>{}(visitee.get(), m_target.get(), m_states);
    }

private:

    std::reference_wrapper<sf::RenderTarget> m_target;
    sf::RenderStates m_states;

} /*class DrawVisitor*/;
