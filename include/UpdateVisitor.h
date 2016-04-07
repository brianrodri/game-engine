#pragma once
#include "Component.h"
#include <type_traits>
#include <utility>

namespace detail {

//! Default behavior, do nothing
template <typename C, typename = void>
struct UpdateVisitorResolverFunctor {

    void operator()(C& visitee, float dt) const
    {
    }

} /*struct UpdateVisitorResolverFunctor*/;

//! Specialized behavior, call update when it exists
template <typename C>
struct UpdateVisitorResolverFunctor<C, decltype(std::declval<C&>().update(std::declval<float>()))> {

    void operator()(C& visitee, float dt) const
    {
        visitee.update(dt);
    }

} /*struct UpdateVisitorResolverFunctor*/;

} /*namespace detail*/;

class UpdateVisitor {
public:

    UpdateVisitor(float dtIn);

    template <typename C>
    void operator()(C& visitee) const
    {
        return detail::UpdateVisitorResolverFunctor<C>{}(visitee, m_dt);
    }

private:
    float m_dt;

} /*struct UpdateVisitor*/;

