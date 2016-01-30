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

template <typename T>
static constexpr const auto sizeof_c(T&&) {
    return index_c<sizeof(T)>;
}

template <typename T>
static constexpr const auto alignof_c(T&&) {
    return index_c<alignof(T)>;
}

// Some useful operators for fun compile-time convinience! :)

//! PLUS
template <typename T, T V, typename U, U W>
constexpr std::integral_constant<std::common_type_t<T, U>, (V + W)>
operator+(std::integral_constant<T, V>, std::integral_constant<U, W>)
{ return {}; }

//! MINUS
template <typename T, T V, typename U, U W>
constexpr std::integral_constant<std::common_type_t<T, U>, (V - W)>
operator-(std::integral_constant<T, V>, std::integral_constant<U, W>)
{ return {}; }

//! TIMES
template <typename T, T V, typename U, U W>
constexpr std::integral_constant<std::common_type_t<T, U>, (V * W)>
operator*(std::integral_constant<T, V>, std::integral_constant<U, W>)
{ return {}; }

//! DIVIDE
template <typename T, T V, typename U, U W>
constexpr std::integral_constant<std::common_type_t<T, U>, (V / W)>
operator/(std::integral_constant<T, V>, std::integral_constant<U, W>)
{ return {}; }

//! MODULO
template <typename T, T V, typename U, U W>
constexpr std::integral_constant<std::common_type_t<T, U>, (V % W)>
operator%(std::integral_constant<T, V>, std::integral_constant<U, W>)
{ return {}; }

//! PRE-INCREMENT
template <typename T, T V>
constexpr std::integral_constant<T, (V + 1)>
operator++(std::integral_constant<T, V>)
{ return {}; }

//! PRE-DECREMENT
template <typename T, T V>
constexpr std::integral_constant<T, (V - 1)>
operator--(std::integral_constant<T, V>)
{ return {}; }

//! EQUALITY COMPARISON
template <typename T, T V, typename U, U W>
constexpr std::bool_constant<(V == W)>
operator==(std::integral_constant<T, V>, std::integral_constant<U, W>)
{ return {}; }

//! INEQUALITY COMPARISON
template <typename T, T V, typename U, U W>
constexpr std::bool_constant<(V != W)>
operator!=(std::integral_constant<T, V>, std::integral_constant<U, W>)
{ return {}; }

//! LESS-THAN COMPARISON
template <typename T, T V, typename U, U W>
constexpr std::bool_constant<(V < W)>
operator<(std::integral_constant<T, V>, std::integral_constant<U, W>)
{ return {}; }

//! GREATER-THAN COMPARISON
template <typename T, T V, typename U, U W>
constexpr std::bool_constant<(V > W)>
operator>(std::integral_constant<T, V>, std::integral_constant<U, W>)
{ return {}; }

//! LESS-THAN-EQUAL COMPARISON
template <typename T, T V, typename U, U W>
constexpr std::bool_constant<(V <= W)>
operator<=(std::integral_constant<T, V>, std::integral_constant<U, W>)
{ return {}; }

//! GREATER-THAN-EQUAL COMPARISON
template <typename T, T V, typename U, U W>
constexpr std::bool_constant<(V >= W)>
operator>=(std::integral_constant<T, V>, std::integral_constant<U, W>)
{ return {}; }

//! LOGICAL NOT
template <typename T, T V>
constexpr std::integral_constant<T, (!V)>
operator!(std::integral_constant<T, V>)
{ return {}; }

//! LOGICAL AND
template <typename T, T V, typename U, U W>
constexpr std::bool_constant<(V && W)>
operator&&(std::integral_constant<T, V>, std::integral_constant<U, W>)
{ return {}; }

//! LOGICAL OR
template <typename T, T V, typename U, U W>
constexpr std::bool_constant<(V || W)>
operator||(std::integral_constant<T, V>, std::integral_constant<U, W>)
{ return {}; }

//! BITWISE AND
template <typename T, T V, typename U, U W>
constexpr std::integral_constant<std::common_type_t<T, U>, (V & W)>
operator&(std::integral_constant<T, V>, std::integral_constant<U, W>)
{ return {}; }

//! BITWISE OR
template <typename T, T V, typename U, U W>
constexpr std::integral_constant<std::common_type_t<T, U>, (V | W)>
operator|(std::integral_constant<T, V>, std::integral_constant<U, W>)
{ return {}; }

//! BITWISE XOR
template <typename T, T V, typename U, U W>
constexpr std::integral_constant<std::common_type_t<T, U>, (V ^ W)>
operator^(std::integral_constant<T, V>, std::integral_constant<U, W>)
{ return {}; }

//! BITWISE COMPLEMENT
template <typename T, T V>
constexpr std::integral_constant<T, ~V>
operator~(std::integral_constant<T, V>)
{ return {}; }

//! SHIFT LEFT
template <typename T, T V, typename U, U W>
constexpr std::integral_constant<std::common_type_t<T, U>, (V << W)>
operator<<(std::integral_constant<T, V>, std::integral_constant<U, W>)
{ return {}; }

//! SHIFT RIGHT
template <typename T, T V, typename U, U W>
constexpr std::integral_constant<std::common_type_t<T, U>, (V >> W)>
operator>>(std::integral_constant<T, V>, std::integral_constant<U, W>)
{ return {}; }
