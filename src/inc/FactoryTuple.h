#ifndef _CPP1Z_GAME_ENGINE_FACTORY_TUPLE_H
#define _CPP1Z_GAME_ENGINE_FACTORY_TUPLE_H
#include <memory>
#include <tuple>
#include <type_traits>
#include <utility>
#include <experimental/array>
#include <experimental/string_view>
#include <cstdlib>

namespace stx = std::experimental;

// FORWARD DECLARATIONS
template <typename... T> class FactoryTuple;



//! Constructs its tuple through factories that use a ref to `this`
/**
 *      A `FactoryTuple` is a special type of tuple that allows its members to
 *  access each other for aid in their construction.  This allows the creation
 *  of tuples of complex types w/ their dependencies kept close together
 *  in-memory, and with minimal memory hoop-jumping as possible.
 */
template <typename... T>
class FactoryTuple {
public:
    using UnderlyingTuple = std::tuple<T...>;
    using Self = FactoryTuple<T...>;

    //! Default Ctor
    /**
     * Only exists when all `T`s are default construible.
     */
    constexpr FactoryTuple()
    {
        ctor_impl(std::index_sequence_for<T...>{});
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
        ctor_impl(std::index_sequence_for<T...>{}, std::forward<F>(f)...);
    }

    //! Destructor
    ~FactoryTuple()
    {
        dtor_impl(std::index_sequence_for<T...>{});
    }

    template <typename I>
    auto& take(I i)
    {
        return id_to_ref(i);
    }

    template <typename I>
    const auto& take(I i) const
    {
        return take(i);
    }

    template <size_t I>
    auto& take()
    {
        return id_to_ref(std::integral_constant<size_t, I>{});
    }

    template <size_t I>
    const auto& take() const
    {
        return take<I>();
    }

    template <typename I>
    auto& operator[](I i)
    {
        return id_to_ref(i);
    }

    template <typename I>
    const auto& operator[](I i) const
    {
        return id_to_ref(i);
    }

    //! FactoryTuple must remain in-place to maintain valid references
    FactoryTuple(Self const&) = delete;
    FactoryTuple(Self&&) = delete;
    Self& operator=(Self const&) = delete;
    Self& operator=(Self&&) = delete;

private:
    //! Assists with compile-time construction semantics
    template <typename U>
    struct MemoryHelper {
    public:
        constexpr MemoryHelper (U& s) : p{&s}
        {
        }

        template <typename... A>
        constexpr void explicit_construct(A&&... arg)
        {
            new(p) U{std::forward<A>(arg)...};
        }

        template <typename Tup, std::size_t... I>
        constexpr void factory_construct(Tup&& args, std::index_sequence<I...>)
        {
            new(p) U{std::get<I>(std::forward<Tup>(args))...};
        }

        constexpr void default_construct()
        {
            new(p) U{};
        }

        constexpr void destruct()
        {
            p->~U();
        }

    private:
        U* p;
    };

    template <typename U>
    constexpr auto makeMemoryHelper(U& src)
    {
        return MemoryHelper<U>{src};
    }

    template <size_t... I, typename... F>
    constexpr void ctor_impl(std::index_sequence<I...>, F&&... fn)
    {
        (
            makeMemoryHelper(
                id_to_ref(std::integral_constant<size_t, I>{})
                ).factory_construct(
                    fn(*this)
                  , std::make_index_sequence<std::tuple_size<std::decay_t<std::result_of_t<F(Self&)>>>::value>{}
                    )
          , ...
        );
    }

    template <size_t ...I>
    constexpr void ctor_impl(std::index_sequence<I...>)
    {
        (
            makeMemoryHelper(id_to_ref(std::integral_constant<size_t, I>{})).default_construct()
          , ...
        );
    }

    template <size_t... I>
    constexpr void dtor_impl(std::index_sequence<I...>)
    {
        (
            makeMemoryHelper(id_to_ref(std::integral_constant<size_t, sizeof...(T) - I - 1>{})).destruct()
          , ...
        );
    }

    template <size_t I>
    std::tuple_element_t<I, std::tuple<T...>>& id_to_ref(std::integral_constant<size_t, I> i)
    {
        using U = std::tuple_element_t<I, std::tuple<T...>>;
        return *reinterpret_cast<U*>(reinterpret_cast<char*>(&m_memory) + generate_member_offset(i));
    }

    template <size_t U = sizeof...(T)>
    static constexpr size_t generate_member_offset(std::integral_constant<size_t, U> upto = {})
    {
        constexpr const size_t alignof_all_v = std::alignment_of<std::aligned_union_t<0, T..., char>>::value;
        constexpr std::array<size_t, sizeof...(T)+1> sz{0, sizeof(T)...}, al{alignof(T)..., alignof_all_v};
        size_t retval{0};
        for (size_t i{0}; i != (U + 1); ++i) {
            retval = ((retval + sz[i] + al[i] - 1) / al[i]) * al[i];
        }
        return retval;
    }

    std::aligned_union_t<generate_member_offset(), T..., char> m_memory;
};

#endif
