#ifndef _CPP1Z_GAME_ENGINE_FACTORY_TUPLE_H
#define _CPP1Z_GAME_ENGINE_FACTORY_TUPLE_H
#include <memory>
#include <tuple>
#include <type_traits>
#include <utility>
#include <boost/hana.hpp>
#include <boost/hana/ext/std/tuple.hpp>

// NAMESPACE ALIASES AND EXPANDERS
namespace hana = boost::hana;
using hana::literals::operator""_c;

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
private:
template <typename U>
struct manual_ctor {
    U *ptr;

    constexpr manual_ctor(U* p) : ptr{p} { }

    constexpr void operator()() const
    {
        new(ptr) U{};
    }

    template <typename Tup>
    constexpr void operator()(Tup&& t) const
    {
        ctor_helper(
            std::forward<Tup>(t)
          , std::make_index_sequence<std::tuple_size<Tup>::value>()
            );
    }

private:
    template <std::size_t... I>
    constexpr void ctor_helper(std::tuple<T&&...>&& a, std::index_sequence<I...>) const
    {
        new(ptr) U{std::forward<T>(std::get<I>(a))...};
    }
};

public:
    using UnderlyingTuple = std::tuple<T...>;
    using Self = FactoryTuple<T...>;
    static constexpr auto TypeList = hana::tuple_t<T...>;

    //! Default Ctor
    /**
     * Only exists when all `T`s are default construible.
     */
    constexpr FactoryTuple()
    {
        hana::length(hana::tuple_t<T...>).times.with_index([=](auto i) {
            using U = std::tuple_element_t<i, UnderlyingTuple>;
            new(&((*this)[i])) U{};
        });
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
        auto factory = hana::make_tuple(std::forward<F>(f)...);
        hana::length(hana::tuple_t<T...>).times.with_index([&](auto i) {
            using U = std::tuple_element_t<i, UnderlyingTuple>;
            manual_ctor<U>{&((*this)[i])}(factory[i](*this));
        });
    }

    //! Destructor
    ~FactoryTuple()
    {
        hana::length(hana::tuple_t<T...>).times.with_index([=](auto i) {
            constexpr auto r = hana::length(hana::tuple_t<T...>) - i - hana::size_c<1>;
            using U = std::tuple_element_t<r, UnderlyingTuple>;
            (*this)[r].~U();
        });
    }

    template <typename I>
    constexpr auto& operator[](I i)
    {
        using U = std::tuple_element_t<i, UnderlyingTuple>;
        constexpr std::ptrdiff_t off{generate_member_offset(i)};
        return *reinterpret_cast<U*>(reinterpret_cast<unsigned char*>(&m_memory) + off);
    }

    template <typename I>
    constexpr const auto& operator[](I i) const
    {
        using U = std::tuple_element_t<i, UnderlyingTuple>;
        constexpr std::ptrdiff_t off{generate_member_offset(i)};
        return *reinterpret_cast<U const*>(reinterpret_cast<unsigned char const*>(&m_memory) + off);
    }

    //! FactoryTuple must remain in-place to maintain valid references
    FactoryTuple(Self const&) = delete;
    FactoryTuple(Self&&) = delete;
    Self& operator=(Self const&) = delete;
    Self& operator=(Self&&) = delete;

private:
    template <std::ptrdiff_t V> 
    static constexpr auto ptrdiff_c = hana::integral_constant<std::ptrdiff_t, V>{};

    struct offhelper {
        constexpr offhelper() = default;

        template <typename S, typename N>
        constexpr auto operator()(S s, N n) const
        {
            return (((s + n[0_c] + n[1_c] - ptrdiff_c<1>) / n[1_c]) * n[1_c]);
        }
    };

    template <typename I = hana::integral_constant<std::size_t, sizeof...(T)>>
    static constexpr auto generate_member_offset(I i = {})
    {
        using namespace hana;
        constexpr std::size_t alignof_all = std::alignment_of<std::aligned_union_t<0, T..., char>>::value;
        constexpr auto sz = prepend(transform(tuple_t<T...>, sizeof_), hana::size_c<0>);
        constexpr auto al = append(transform(tuple_t<T...>, alignof_), hana::size_c<alignof_all>);
        constexpr auto seq = take(drop_front(zip(sz, al)), i);
        return fold(seq, ptrdiff_c<0>, offhelper{});
    }

    std::aligned_union_t<generate_member_offset(), T..., char> m_memory;
};

#endif
