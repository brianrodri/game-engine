#pragma once
#include <type_traits>

template <size_t I> using index_constant = std::integral_constant<size_t, I>;
template <size_t I> constexpr auto index_c = index_constant<I>{};

template <bool B> static constexpr auto bool_c = std::bool_constant<B>{};


namespace detail {

template <typename ForwardIt>
constexpr size_t _c_impl_(ForwardIt first, ForwardIt last)
{
    size_t ret{0};
    for (auto it = first; it != last; ++it) { ret = (ret * 10) + (*it - '0'); }
    return ret;
}

constexpr bool isdigit(char c)
{
    return '0' <= c <= '9';
}

}; // namespace detail

template <char... C>
constexpr auto operator ""_c ()
{
    constexpr char str[]{C...};
    return index_c<detail::_c_impl_(str, str + sizeof...(C))>;
}

