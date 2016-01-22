#pragma once
#include <functional>
#include <memory>
#include <utility>
#include <experimental/array>
#include <experimental/type_traits>
#include <experimental/tuple>
#include "my_literals.h"


//! Constructs its tuple through factories that use a ref to `this`
/**
 *      A `FactoryTuple` is a special type of tuple that allows its members to
 *  access each other for aid in their construction.  This allows the creation
 *  of tuples of complex types w/ their dependencies kept close together
 *  in-memory, and with minimal memory hoop-jumping as possible.
 */
template <typename... T>
class FactoryTuple {
    using Self = FactoryTuple<T...>;
    using Tuple = std::tuple<T...>;

public:
    //! Default Ctor
    /**
     * Only exists when all `T`s are default construible.
     */
    constexpr FactoryTuple() noexcept
    {
        call_constructor_w(std::index_sequence_for<T...>{});
    }

    //! Factory Ctor
    /**
     * Takes a series of `F` callables and uses their resultant tuple to
     * construct each `T`.  `F`s are called in such a way that they can refer to
     * earlier constructed `T` values.
     *
     * Validity of each `F` is checked by the `manual_ctor_for` struct
     */
    template<typename... F>
        //requires Function<F, Self&> && ...
    constexpr FactoryTuple(F&&... f) noexcept
    {
        call_constructor_w(std::index_sequence_for<T...>{}, std::forward<F>(f)...);
    }

    //! Destructor
    ~FactoryTuple() noexcept
    {
        call_destructor_w(std::index_sequence_for<T...>{});
    }

    //! FactoryTuple must remain in-place to maintain valid references
    FactoryTuple(Self const&) = delete;
    FactoryTuple(Self&&) = delete;
    Self& operator=(Self const&) = delete;
    Self& operator=(Self&&) = delete;

    //! Converts elements to a tied tuple
    constexpr auto as_tuple() noexcept
    {
        return tie(std::index_sequence_for<T...>{});
    }

    //! Converts elements to a const tied tuple noexcept
    constexpr auto as_tuple() const noexcept
    {
        return tie(std::index_sequence_for<T...>{});
    }

    //! Creates a copy of the inner elements
    constexpr auto make_tuple() const noexcept
    {
        static_assert((std::experimental::is_copy_constructible_v<T> && ...));
        return copy(std::index_sequence_for<T...>{});
    }

    template <size_t I>
    constexpr auto& operator[](index_constant<I> i) noexcept
    {
        return id_to_ref(i);
    }

    template <size_t I>
    constexpr const auto& operator[](index_constant<I> i) const noexcept
    {
        return id_to_ref(i);
    }

private:
    template <size_t... I>
    constexpr auto tie(std::index_sequence<I...> i) noexcept
    {
        return std::tie(id_to_ref(index_c<I>)...);
    }

    template <size_t... I>
    constexpr auto tie(std::index_sequence<I...> i) const noexcept
    {
        return std::tie(id_to_ref(index_c<I>)...);
    }

    template <size_t... I>
    constexpr auto copy(std::index_sequence<I...> i) const noexcept
    {
        return Tuple{id_to_ref(index_c<I>)...};
    }

    template <size_t... I, typename... F>
    constexpr void call_constructor_w(std::index_sequence<I...>, F&&... f) noexcept
    {
        static_assert(sizeof...(I) == sizeof...(F));
        (
            construct(
                index_c<I>
              , f(*this)
              , std::make_index_sequence<std::experimental::tuple_size_v<std::result_of_t<F(Self&)>>>{}
                ) // construct
          , ...
            );
    }

    template <size_t... I>
    constexpr void call_constructor_w(std::index_sequence<I...>) noexcept
    {
        (construct(index_c<I>), ...);
    }

    template <size_t... I>
    constexpr void call_destructor_w(std::index_sequence<I...>) noexcept
    {
        (destruct(index_c<sizeof...(T) - I - 1>), ...);
    }

    template <size_t I>
    constexpr void construct(index_constant<I> i) noexcept
    {
        using U = std::tuple_element_t<I, Tuple>;
        static_assert(std::experimental::is_default_constructible_v<U>);
        U* ptr = &id_to_ref(i);
        new(ptr) U{};
    }

    template <size_t I, typename Yield, size_t... J>
    constexpr void construct(index_constant<I> i, Yield&& y, std::index_sequence<J...>) noexcept
    {
        using U = std::tuple_element_t<I, Tuple>;
        static_assert(std::experimental::is_constructible_v<U, std::decay_t<std::tuple_element_t<J, Yield>>...>);
        U* ptr = &id_to_ref(i);
        new(ptr) U(std::forward<std::decay_t<std::tuple_element_t<J, Yield>>>(std::get<J>(std::forward<Yield>(y)))...);
    }

    template <size_t I>
    constexpr void destruct(index_constant<I> i) noexcept
    {
        using U = std::tuple_element_t<i, Tuple>;
        U* ptr = &id_to_ref(i);
        ptr->~U();
    }

    template <size_t I>
    constexpr std::tuple_element_t<I, Tuple>& id_to_ref(index_constant<I> i) noexcept
    {
        using U = std::tuple_element_t<I, Tuple>;
        return *reinterpret_cast<U*>(reinterpret_cast<char*>(&m_memory) + id_to_off(i));
    }

    template <size_t I>
    constexpr const std::tuple_element_t<I, Tuple>& id_to_ref(index_constant<I> i) const noexcept
    {
        using U = std::tuple_element_t<I, Tuple>;
        return *reinterpret_cast<const U*>(reinterpret_cast<const char*>(&m_memory) + id_to_off(i));
    }

    template <size_t U = sizeof...(T)>
    static constexpr std::ptrdiff_t id_to_off(index_constant<U> upto = {}) noexcept
    {
        constexpr size_t N = U + 1;
        constexpr auto sz = std::experimental::make_array(0, sizeof(T)...);
        constexpr auto al = std::experimental::make_array(
            alignof(T)...
          , std::experimental::alignment_of_v<std::aligned_union_t<0, T..., char>>
            );
        size_t retval{0};
        for (size_t i{0}; i != N; ++i) {
            retval = ((retval + sz[i] + al[i] - 1) / al[i]) * al[i];
        }
        return retval;
    }

    std::aligned_union_t<id_to_off(), T..., char> m_memory;
};

namespace std {

template <typename... T, size_t I>
constexpr auto& get(FactoryTuple<T...>& o)
{
    return o[index_c<I>];
}

template <typename... T, size_t I>
constexpr const auto& get(const FactoryTuple<T...>& o)
{
    return o[index_c<I>];
}

template <typename... T>
struct tuple_size<FactoryTuple<T...>> {
    static constexpr size_t value = sizeof...(T);
};

} // namespace std
