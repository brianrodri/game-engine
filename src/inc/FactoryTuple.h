#pragma once
#include <functional>
#include <memory>
#include <utility>
#include <tuple>
#include <experimental/array>
#include <experimental/type_traits>
#include <experimental/tuple>
#include "index_constant.h"
#include "type_constant.h"



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
    using TiedTuple = std::tuple<T&...>;
    using ConstTiedTuple = std::tuple<const T&...>;

public:
    //! Default Ctor
    /**
     * Only exists when all `T`s are default constructible.
     */
    constexpr FactoryTuple() noexcept
    {
        callConstructorWith(std::index_sequence_for<T...>{});
    }

    //! Factory Ctor
    /**
     * Takes a series of `F` callables and uses their resultant tuple to
     * construct each `T`.  `F`s are called in such a way that they may refer to
     * earlier validly constructed `T` values.
     *
     * Validity of each `F` is checked by the `construct` template member
     * function
     */
    template<typename... F>
        //requires Function<F, Self&> && ...
    constexpr FactoryTuple(F&&... f) noexcept
    {
        callConstructorWith(std::index_sequence_for<T...>{}, std::forward<F>(f)...);
    }

    constexpr operator std::tuple<T...>() const
    {
        return tieImpl(std::index_sequence_for<T...>{});
    }

    //! Destructor
    ~FactoryTuple() noexcept
    {
        callDestructorWith(std::index_sequence_for<T...>{});
    }

    //! FactoryTuple must remain in-place to maintain valid references
    FactoryTuple(Self const&) = delete;
    FactoryTuple(Self&&) = delete;
    Self& operator=(Self const&) = delete;
    Self& operator=(Self&&) = delete;

    //! Allows compile-time access through an index into the typelist
    template <size_t I>
    constexpr auto& operator[](index_constant<I> i) noexcept
    {
        return idxToRef(i);
    }

    //! Allows constant compile-time access through an index into the typelist
    template <size_t I>
    constexpr const auto& operator[](index_constant<I> i) const noexcept
    {
        return idxToCRef(i);
    }

    //! Allows compile-time access through an element of the typelist
    template <typename U>
    constexpr auto& operator[](type_constant<U> t) noexcept
    {
        return idxToRef(type_index_v<U, T...>);
    }

    //! Allows constant compile-time access through an element of the typelist
    template <typename U>
    constexpr const auto& operator[](type_constant<U> t) const noexcept
    {
        return idxToCRef(type_index_v<U, T...>);
    }

private:
    template <size_t... I>
    constexpr auto tieImpl(std::index_sequence<I...> i) const noexcept
    {
        return std::make_tuple(
            std::tuple_element_t<I, Tuple>(idxToCRef(index_c<I>)).. .
            );
    }

    template <size_t... I, typename... F>
    constexpr void callConstructorWith(std::index_sequence<I...>, F&&... f) noexcept
    {
        static_assert(sizeof...(I) == sizeof...(F));
        (
            construct(
                index_c<I>
              , type_c<T>
              , f(*this)
              , std::make_index_sequence<std::experimental::tuple_size_v<std::result_of_t<F(Self&)>>>{}
                ) // construct
          , ...
            );
    }

    template <size_t... I>
    constexpr void callConstructorWith(std::index_sequence<I...>) noexcept
    {
        (construct(index_c<I>, type_c<T>), ...);
    }

    template <size_t... I>
    constexpr void callDestructorWith(std::index_sequence<I...>) noexcept
    {
        (destruct(
            index_c<sizeof...(T) - I - 1>
          , type_c<std::tuple_element_t<sizeof...(T) - I - 1, Tuple>>
            ), ...
        );
    }

    template <size_t I, typename U>
    constexpr void construct(index_constant<I> i, type_constant<U> t) noexcept
    {
        static_assert(std::experimental::is_default_constructible_v<U>);
        new(&idxToRef(i)) U();
    }

    template <size_t I, typename U, typename Yield, size_t... J>
    constexpr void construct(index_constant<I> i, type_constant<U> t, Yield&& y, std::index_sequence<J...>) noexcept
    {
        static_assert(std::experimental::is_constructible_v<U, std::decay_t<std::tuple_element_t<J, Yield>>...>);
        new(&idxToRef(i)) U(std::forward<std::decay_t<std::tuple_element_t<J, Yield>>>(std::get<J>(std::forward<Yield>(y)))...);
    }

    template <size_t I, typename U>
    constexpr void destruct(index_constant<I> i, type_constant<U> t) noexcept
    {
        idxToRef(i).~U();
    }

    template <size_t I>
    constexpr const std::tuple_element_t<I, Tuple>& idxToCRef(index_constant<I> i) const noexcept
    {
        using U = std::tuple_element_t<I, Tuple>;
        return *reinterpret_cast<const U*>(reinterpret_cast<const char*>(&m_memory) + idxToOff(i));
    }

    template <size_t I>
    constexpr std::tuple_element_t<I, Tuple>& idxToRef(index_constant<I> i) noexcept
    {
        using U = std::tuple_element_t<I, Tuple>;
        return *reinterpret_cast<U*>(reinterpret_cast<char*>(&m_memory) + idxToOff(i));
    }

    template <size_t U = sizeof...(T)>
    static constexpr std::ptrdiff_t idxToOff(index_constant<U> upto = {}) noexcept
    {
        constexpr auto sz = std::experimental::make_array(0, sizeof(T)...);
        constexpr auto al = std::experimental::make_array(
            alignof(T)...
          , std::experimental::alignment_of_v<std::aligned_union_t<0, T..., char>>
            );
        std::ptrdiff_t retval{0};
        for (size_t i{1}; i != (U + 1); ++i) {
            retval = ((retval + sz[i] + al[i] - 1) / al[i]) * al[i];
        }
        return retval;
    }

    std::aligned_union_t<idxToOff(), T..., char> m_memory;
};

namespace std {

template <size_t I, typename... T>
constexpr std::tuple_element_t<I, std::tuple<T...>>& get(FactoryTuple<T...>& o)
{
    return o[index_c<I>];
}

template <size_t I, typename... T>
constexpr const std::tuple_element_t<I, std::tuple<T...>>& get(const FactoryTuple<T...>& o)
{
    return o[index_c<I>];
}

template <typename U, typename... T>
constexpr U& get(FactoryTuple<T...>& o)
{
    return o[type_c<U>];
}

template <typename U, typename... T>
constexpr const U& get(const FactoryTuple<T...>& o)
{
    return o[type_c<U>];
}

template <typename... T>
struct tuple_size<FactoryTuple<T...>> {
    static constexpr auto value = index_c<sizeof...(T)>;
};

} // namespace std
