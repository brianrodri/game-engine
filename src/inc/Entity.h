#pragma once
#include "Component.h"
#include "FactoryTuple.h"
#include "index_constant.h"
#include <type_traits>

namespace stx = std::experimental;

template <typename... C>
class Entity {
    static_assert(std::is_base_of_v<C, Component> && ...);

public:
    template <typename... F>
    constexpr Entity(F&&... f) : storage{std::forward<F>(f)...}
    {
    }

    ~Entity()
    {
    }

    template <size_t I>
    constexpr auto& operator[](index_constant<I> i)
    {
        return storage[i];
    }

    template <size_t I>
    constexpr const auto& operator[](index_constant<I> i) const
    {
        return storage[i];
    }

    template <typename U>
    constexpr auto& operator[](type_constant<U> t)
    {
        return storage[t];
    }

    template <typename U>
    constexpr const auto& operator[](type_constant<U> t) const
    {
        return storage[t];
    }

private:
    FactoryTuple<C...> storage;
};
