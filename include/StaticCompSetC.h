#pragma once
#include <experimental/type_traits>
#include "Component.h"
#include "FactoryTuple.h"
#include <aetee/aetee.h>

//! Designed to compose several _related_ components as one functioning component
template <typename... C>
class StaticCompSetC : public Component {
    static_assert(std::conjunction<std::is_base_of<BaseComponent, C>...>::value);

    struct updateChecker {
        template <size_t I> constexpr auto operator()(aetee::index_constant_t<I>) const
        {
            using K = aetee::type_at_t<I, C...>;
            return std::disjunction< std::is_base_of<ProcessorComponent, K>
                                   , std::is_base_of<Component, K>
                                     >{};
        }
    } /*struct updateChecker*/;

    struct renderChecker {
        template <size_t I> constexpr auto operator()(aetee::index_constant_t<I>) const
        {
            using K = aetee::type_at_t<I, C...>;
            return std::disjunction< std::is_base_of<PainterComponent, K>
                                   , std::is_base_of<Component, K>
                                     >{};
        }
    } /*struct renderChecker*/;

public:
    //! Default behavior
    StaticCompSetC() = default;
    virtual ~StaticCompSetC() = default;

    //! Can not be moved in any way due to possible complexities
    StaticCompSetC(const StaticCompSetC&) = delete;
    StaticCompSetC(StaticCompSetC&&) = delete;
    StaticCompSetC& operator=(const StaticCompSetC&) = delete;
    StaticCompSetC& operator=(StaticCompSetC&&) = delete;

    //! Explicit construction w/ factories
    template <typename... F>
    constexpr StaticCompSetC(F&&... f) : members{std::forward<F>(f)...}
    {
    }

    template <size_t I>
    auto& operator[](aetee::index_constant_t<I> i)
    {
        return members[i];
    }

    template <size_t I>
    const auto& operator[](aetee::index_constant_t<I> i) const
    {
        return members[i];
    }

    auto& as_tuple()
    {
        return members.as_tuple();
    }

    const auto& as_tuple() const
    {
        return members.as_tuple();
    }

    virtual void update(float dt) override
    {
        aetee::for_each(
            aetee::filter(aetee::index_sequence_c_for<C...>, updateChecker{})
          , [=](auto i) { this->operator[](i).update(dt); }
            );
    }

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override
    {
        aetee::for_each(
            aetee::filter(aetee::index_sequence_c_for<C...>, renderChecker{})
          , [&](auto i) { this->operator[](i).render(target, {states}); }
            );
    }

private:
    FactoryTuple<C...>  members;
};
