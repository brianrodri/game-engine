#pragma once
#include <algorithm>
#include <string>
#include <stdlib.h>
#include <type_traits>

template <typename I>
constexpr size_t _c_helper(I first, I last)
{
    size_t v{0};
    for (auto it = first; it != last; ++it) {
        v = (v * 10) + (*it - '0');
    }
    return v;
}

template <char... C>
constexpr auto operator ""_c ()
{
    constexpr char str[]{C...};
    return std::integral_constant<size_t, _c_helper(str, str + sizeof...(C))>{};
}
