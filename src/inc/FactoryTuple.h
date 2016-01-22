#ifndef _CPP1Z_GAME_ENGINE_FACTORY_TUPLE_H
#define _CPP1Z_GAME_ENGINE_FACTORY_TUPLE_H
#include <cstdlib>
#include <functional>
#include <memory>
#include <tuple>
#include <type_traits>
#include <utility>
#include <experimental/array>
#include <experimental/type_traits>
#include <experimental/tuple>
#include <experimental/string_view>

namespace stx = std::experimental;


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
    constexpr FactoryTuple()
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
    constexpr FactoryTuple(F&&... f)
    {
        call_constructor_w(std::index_sequence_for<T...>{}, std::forward<F>(f)...);
    }

    //! Destructor
    ~FactoryTuple()
    {
        call_destructor_w(std::index_sequence_for<T...>{});
    }

    //! FactoryTuple must remain in-place to maintain valid references
    FactoryTuple(Self const&) = delete;
    FactoryTuple(Self&&) = delete;
    Self& operator=(Self const&) = delete;
    Self& operator=(Self&&) = delete;

    constexpr auto as_tuple()
    {
        return tie(std::index_sequence_for<T...>{});
    }

    constexpr auto as_tuple() const
    {
        return tie(std::index_sequence_for<T...>{});
    }

    constexpr auto make_tuple() const
    {
        return copy(std::index_sequence_for<T...>{});
    }

    template <typename I>
    constexpr auto& operator[](I i)
    {
        return id_to_ref(i);
    }

    template <typename I>
    constexpr const auto& operator[](I i) const
    {
        return id_to_ref(i);
    }

private:
    template <size_t... I>
    constexpr auto tie(std::index_sequence<I...> i)
    {
        return std::tie(id_to_ref(std::integral_constant<size_t, I>{})...);
    }

    template <size_t... I>
    constexpr auto tie(std::index_sequence<I...> i) const
    {
        return std::tie(id_to_ref(std::integral_constant<size_t, I>{})...);
    }

    template <size_t... I>
    constexpr auto copy(std::index_sequence<I...> i) const
    {
        return Tuple{id_to_ref(std::integral_constant<size_t, I>{})...};
    }

    template <size_t... I, typename... F>
    constexpr void call_constructor_w(std::index_sequence<I...>, F&&... f)
    {
        static_assert(sizeof...(I) == sizeof...(F));
        (
            construct(
                std::integral_constant<size_t, I>{}
              , f(*this)
              , std::make_index_sequence<stx::tuple_size_v<std::decay_t<std::result_of_t<F(Self&)>>>>{}
                ) // construct
          , ...
            );
    }

    template <size_t... I>
    constexpr void call_constructor_w(std::index_sequence<I...>)
    {
        (construct(std::integral_constant<size_t, I>{}), ...);
    }

    template <size_t... I>
    constexpr void call_destructor_w(std::index_sequence<I...>)
    {
        (destruct(std::integral_constant<size_t, sizeof...(T) - I - 1>{}), ...);
    }

    template <typename I, typename Yield, size_t... J>
    constexpr void construct(I i, Yield&& y, std::index_sequence<J...>)
    {
        using U = std::tuple_element_t<i, Tuple>;
        static_assert(stx::is_constructible_v<U, std::tuple_element_t<J, Yield>...>);
        U* ptr = &id_to_ref(i);
        new(ptr) U{std::move(std::get<J>(std::forward<Yield>(y)))...};
    }

    template <typename I>
    constexpr void construct(I i)
    {
        using U = std::tuple_element_t<i, Tuple>;
        static_assert(stx::is_default_constructible_v<U>);
        U* ptr = &id_to_ref(i);
        new(ptr) U{};
    }

    template <typename I>
    constexpr void destruct(I i)
    {
        using U = std::tuple_element_t<i, Tuple>;
        U* ptr = &id_to_ref(i);
        ptr->~U();
    }

    template <size_t I>
    constexpr std::tuple_element_t<I, Tuple>& id_to_ref(std::integral_constant<size_t, I> i)
    {
        using U = std::tuple_element_t<I, Tuple>;
        return *reinterpret_cast<U*>(reinterpret_cast<char*>(&m_memory) + id_to_off(i));
    }

    template <size_t I>
    constexpr const std::tuple_element_t<I, Tuple>& id_to_ref(std::integral_constant<size_t, I> i) const
    {
        using U = std::tuple_element_t<I, Tuple>;
        return *reinterpret_cast<const U*>(reinterpret_cast<const char*>(&m_memory) + id_to_off(i));
    }

    template <size_t U = sizeof...(T)>
    static constexpr std::ptrdiff_t id_to_off(std::integral_constant<size_t, U> upto = {})
    {
        constexpr size_t N = U + 1;
        constexpr auto sz = stx::make_array(0, sizeof(T)...);
        constexpr auto al = stx::make_array(
                alignof(T)...
              , stx::alignment_of_v<std::aligned_union_t<0, T..., char>>
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
    return o[std::integral_constant<size_t, I>{}];
}

template <typename... T, size_t I>
constexpr const auto& get(const FactoryTuple<T...>& o)
{
    return o[std::integral_constant<size_t, I>{}];
}

template <typename... T>
struct tuple_size<FactoryTuple<T...>> {
    static constexpr size_t value = sizeof...(T);
};

} // namespace std

#endif
