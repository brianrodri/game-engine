#pragma once
#include <experimental/type_traits>
#include "Component.h"
#include "FactoryTuple.h"
#include <aetee/aetee.h>

//! Designed to compose several _related_ components as one functioning component
template <typename... C>
class StaticSetC : public Component {
    static_assert(std::conjunction<std::is_base_of<BaseComponent, C>...>::value);

public:
    //! Default behavior
    StaticSetC() = default;
    virtual ~StaticSetC() = default;

    //! Can not be moved in any way due to possible complexities
    StaticSetC(const StaticSetC&) = delete;
    StaticSetC(StaticSetC&&) = delete;
    StaticSetC& operator=(const StaticSetC&) = delete;
    StaticSetC& operator=(StaticSetC&&) = delete;

    //! Explicit construction w/ factories
    template <typename... F>
    constexpr StaticSetC(F&&... f) : members{std::forward<F>(f)...}
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
            aetee::filter(
                aetee::index_sequence_c_for<C...>
              , [](auto i) {
                    return std::is_base_of<ProcessorComponent, aetee::type_at_t<i, C...>>{};
                })
          , [=](auto i) { this->operator[](i).update(dt); }
            );
    }

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override
    {
        aetee::for_each(
            aetee::filter(
                aetee::index_sequence_c_for<C...>
              , [](auto i) {
                    return std::is_base_of<PainterComponent, aetee::type_at_t<i, C...>>{};
                })
          , [&](auto i) { this->operator[](i).draw(target, states); }
            );
    }

private:
    FactoryTuple<C...>  members;
};
