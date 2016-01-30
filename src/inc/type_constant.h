#pragma once
#include "index_constant.h"

//! type_constant
template <typename T> struct type_constant { using type = T; };
template <typename T> constexpr auto type_c = type_constant<T>{};

//! type_exists
template <typename H, typename... T>
struct type_exists : std::experimental::disjunction<std::is_same<H, T>...> {};

template <typename H, typename... T>
static constexpr auto type_exists_v = type_exists<H, T...>::value;

//! type_index
namespace detail {

template <typename H, typename... T>
struct type_index_impl;

template <typename H>
struct type_index_impl<H> : index_constant<0> {};

template <typename H, typename... T>
struct type_index_impl<H, H, T...> : index_constant<0> {};

template <typename H, typename X, typename... T>
struct type_index_impl<H, X, T...> : index_constant<1 + type_index_impl<H, T...>::value> {};

}; // namespace detail

template <typename T, typename... L>
using type_index = detail::type_index_impl<T, L...>;

template <typename T, typename... L>
static constexpr auto type_index_v = type_index<T, L...>{};
