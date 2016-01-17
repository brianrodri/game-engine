#ifndef _CPP1Z_GAME_ENGINE_FACTORY_TUPLE_H
#define _CPP1Z_GAME_ENGINE_FACTORY_TUPLE_H
#include <functional>
#include <memory>
#include <tuple>
#include <type_traits>
#include <utility>
#include <experimental/array>
#include <experimental/type_traits>
#include <experimental/tuple>
#include <experimental/string_view>
#include <cstdlib>

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
    static const constexpr auto IDXES = std::index_sequence_for<T...>{};

public:
    //! Default Ctor
    /**
     * Only exists when all `T`s are default construible.
     */
    constexpr FactoryTuple()
    {
        ctor_impl(IDXES);
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
        static_assert(sizeof...(F) == sizeof...(T), "!");
        ctor_impl(IDXES, std::forward<F>(f)...);
    }

    //! Destructor
    ~FactoryTuple()
    {
        dtor_impl(IDXES);
    }

    //! FactoryTuple must remain in-place to maintain valid references
    FactoryTuple(Self const&) = delete;
    FactoryTuple(Self&&) = delete;
    Self& operator=(Self const&) = delete;
    Self& operator=(Self&&) = delete;

    std::tuple<T&...> as_tuple()
    {
        return tie_refs_to(IDXES);
    }

    std::tuple<const T&...> as_tuple() const
    {
        return tie_refs_to(IDXES);
    }

    auto to_tuple() const
    {
        return make_vals_of(IDXES);
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

private:
    template <size_t... I>
    auto tie_refs_to(std::index_sequence<I...> i)
    {
        return std::tie(id_to_ref(std::integral_constant<size_t, I>{})...);
    }

    template <size_t... I>
    auto tie_refs_to(std::index_sequence<I...> i) const
    {
        return tie_refs_to(i);
    }

    template <size_t... I>
    auto make_vals_of(std::index_sequence<I...> i) const
    {
        return std::make_tuple(id_to_ref(std::integral_constant<size_t, I>{})...);
    }

    template <size_t I>
    friend struct MemoryHelper;

    //! Assists with compile-time construction semantics
    template <size_t I>
    struct MemoryHelper {
    private:
        using U = std::tuple_element_t<I, std::tuple<T...>>;
        static constexpr auto i = std::integral_constant<size_t, I>{};

    public:
        MemoryHelper(Self& obj, std::integral_constant<size_t, I>) : myref{obj}
        {
        }

        void default_construct()
        {
            U* p = &myref.get()[i];
            new(p) U{};
        }

        template <typename... A>
        void explicit_construct(A&&... arg)
        {
            static_assert(stx::is_constructible_v<U, A...>);
            U* p = &myref.get()[i];
            new(p) U{std::forward<A>(arg)...};
        }

        template <typename F, std::size_t... J>
        void factory_construct(F&& f, std::index_sequence<J...>)
        {
            auto yielding = f(myref.get());
            using Y = std::decay_t<std::result_of_t<F(Self&)>>;
            explicit_construct(std::forward<std::tuple_element_t<J, Y>>(std::get<J>(yielding))...);
        }

        void destruct()
        {
            myref.get()[i].~U();
        }

    private:
        std::reference_wrapper<Self> myref;
    };

    template <typename I>
    auto makeMemoryHelper(I i)
    {
        return MemoryHelper<i>{*this, i};
    }

    template <typename... F, size_t... I>
    void ctor_impl(std::index_sequence<I...>, F&&... fn)
    {
        ( makeMemoryHelper(std::integral_constant<size_t, I>{}).factory_construct(
             std::forward<F>(fn)
           , std::make_index_sequence<std::tuple_size<std::decay_t<std::result_of_t<F(Self&)>>>::value>{}
             )
          , ...
        );
    }

    template <size_t... I>
    void ctor_impl(std::index_sequence<I...>)
    {
        (
            makeMemoryHelper(std::integral_constant<size_t, I>{}).default_construct()
          , ...
        );
    }

    template <size_t... I>
    void dtor_impl(std::index_sequence<I...>)
    {
        (
            makeMemoryHelper(std::integral_constant<size_t, sizeof...(T) - I - 1>{}).destruct()
          , ...
        );
    }

    template <size_t I>
    auto& id_to_ref(std::integral_constant<size_t, I> i)
    {
        using U = std::tuple_element_t<I, std::tuple<T...>>;
        return *reinterpret_cast<U*>(reinterpret_cast<char*>(&m_memory) + id_to_off(i));
    }

    template <size_t I>
    const auto& id_to_ref(std::integral_constant<size_t, I> i) const
    {
        using U = std::tuple_element_t<I, std::tuple<T...>>;
        return *reinterpret_cast<const U*>(reinterpret_cast<const char*>(&m_memory) + id_to_off(i));
    }

    template <size_t U = sizeof...(T)>
    static constexpr size_t id_to_off(std::integral_constant<size_t, U> upto = {})
    {
        constexpr std::array<size_t, sizeof...(T)+1> sz{0, sizeof(T)...};
        constexpr std::array<size_t, sizeof...(T)+1> al{alignof(T)..., stx::alignment_of_v<std::aligned_union_t<0, T..., char>>};
        size_t retval{0};
        for (size_t i{0}; i != (U + 1); ++i) {
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
