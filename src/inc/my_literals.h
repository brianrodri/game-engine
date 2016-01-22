#pragma once
#include <algorithm>
#include <string>
#include <stdlib.h>
#include <type_traits>

namespace detail {

template <typename ForwardIt>
constexpr size_t _c_impl_(ForwardIt first, ForwardIt last)
{
    size_t ret{0};
    for (auto it = first; it != last; ++it) { ret = (ret * 10) + (*it - '0'); }
    return ret;
}

}; // anon namespace

template <char... C>
constexpr auto operator ""_c ()
{
    constexpr char str[]{C...};
    return std::integral_constant<size_t, detail::_c_impl_(str, str + sizeof...(C))>{};
}

template <typename T>
struct type_constant { using type = T; };

template <typename T>
constexpr auto type_c = type_constant<T>{};

template <size_t I>
using index_constant = std::integral_constant<size_t, I>;

template <size_t I>
constexpr auto index_c = index_constant<I>{};
