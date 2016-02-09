#pragma once
#include <functional>
#include <memory>
#include <utility>
#include <experimental/array>
#include <experimental/type_traits>
#include <experimental/tuple>
#include <aetee/aetee.h>



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
public:
    //! Default Ctor
    /**
     * Only exists when all `T`s are default constructible.
     */
    constexpr FactoryTuple()
    {
        using namespace aetee;
        defaultConstruct(index_sequence_c_for<T...>);
    }

    //! Factory Ctor
    template <typename... F>
    constexpr FactoryTuple(F&&... f)
    {
        using namespace aetee;
        static_assert(arity_c<F...> == arity_c<T...>);
        factoryConstruct(index_sequence_c_for<F...>, std::forward<F>(f)...);
    }

    //! Destructor
    ~FactoryTuple() 
    {
        using namespace aetee;
        destruct(reverse(index_sequence_c_for<T...>));
    }

    //! FactoryTuple must remain in-place to maintain valid references
    FactoryTuple(Self const&) = delete;
    FactoryTuple(Self&&) = delete;
    Self& operator=(Self const&) = delete;
    Self& operator=(Self&&) = delete;

    //! Allows an explicit cast to a `std::tuple<U...>`
    /**
     * I made this to make testing easier mostly, but also because I feel this is the
     * most expressive way of saying "treat this as a tuple," and it true to
     * the theme of operator-access only, which I like.
     */
    template <typename... U>
    constexpr explicit operator std::tuple<U...>() const
    {
        using namespace aetee;
        return tie(index_sequence_c_for<U...>);
    }

    //! Allows compile-time access through an index into the typelist
    template <size_t I>
    constexpr auto& operator[](aetee::index_constant_t<I> i) 
    {
        using namespace aetee;
        constexpr auto off = offsetof_(type_sequence_c<T...>, i);
        char * raw = reinterpret_cast<char *>(&m_memory) + off;
        return *reinterpret_cast<type_at_t<I, T...> *>(raw);
    }

    //! Allows constant compile-time access through an index into the typelist
    template <size_t I>
    constexpr const auto& operator[](aetee::index_constant_t<I> i) const 
    {
        using namespace aetee;
        constexpr auto off = offsetof_(type_sequence_c<T...>, i);
        const char * raw = reinterpret_cast<const char *>(&m_memory) + off;
        return *reinterpret_cast<const type_at_t<I, T...> *>(raw);
    }

    //! Allows compile-time access through an element of the typelist
    template <typename U>
    constexpr auto& operator[](aetee::type_constant_t<U> t) 
    {
        return operator[](aetee::type_index_c<U, T...>);
    }

    //! Allows constant compile-time access through an element of the typelist
    template <typename U>
    constexpr const auto& operator[](aetee::type_constant_t<U> t) const 
    {
        return operator[](aetee::type_index_c<U, T...>);
    }

private:
    template <size_t I, typename... A>
    constexpr void explicitConstruct(aetee::index_constant_t<I> i, A&&... args)
    {
        using namespace aetee;
        using U = type_at_t<I, T...>;
        U *location = &operator[](i);
        new(location) U{std::forward<A>(args)...};
    }

    template <size_t... I>
    constexpr void defaultConstruct(aetee::index_sequence_t<I...>)
    {
        using namespace aetee;
        (explicitConstruct(index_c<I>), ...);
    }

    template <size_t... I, typename... F>
    constexpr void factoryConstruct(aetee::index_sequence_t<I...>, F&&... f)
    {
        using namespace aetee;
        (factoryConstruct(index_c<I>, index_sequence_c<std::tuple_size<std::result_of_t<F(Self&)>>::value>, f(*this)), ...);
    }

    template <size_t I, size_t... J, typename Yield>
    constexpr void factoryConstruct(aetee::index_constant_t<I> i, aetee::index_sequence_t<J...> js, Yield&& y)
    {
        using namespace aetee;
        explicitConstruct(i, std::get<J>(std::forward<Yield>(y))...);
    }

    template <size_t... I>
    constexpr void destruct(aetee::index_sequence_t<I...>)
    {
        using namespace aetee;
        (operator[](index_c<I>).~type_at_t<I, T...>(), ...);
    }

    template <size_t... I>
    constexpr auto tie(aetee::index_sequence_t<I...>) const
    {
        using namespace aetee;
        return std::tie(operator[](index_c<I>)...);
    }

    static constexpr const size_t Len = aetee::offsetof_(aetee::type_sequence_c<T...>);
    static constexpr const size_t Align = aetee::alignofall(aetee::type_sequence_c<T...>);
    std::aligned_storage_t<Len, Align> m_memory;
} /*class FactoryTuple*/;

namespace std {

template <size_t I, typename... T>
    constexpr auto& get(FactoryTuple<T...>& o) { return o[aetee::index_c<I>]; }
template <size_t I, typename... T>
    constexpr const auto& get(const FactoryTuple<T...>& o) { return o[aetee::index_c<I>]; }
template <typename U, typename... T>
    constexpr auto& get(FactoryTuple<T...>& o) { return o[aetee::type_c<U>]; }
template <typename U, typename... T>
    constexpr const auto& get(const FactoryTuple<T...>& o) { return o[aetee::type_c<U>]; }

template <typename... T>
struct tuple_size<FactoryTuple<T...>> {
    static constexpr auto value = aetee::arity_c<T...>;
} /*struct tuple_size*/;

} /*namespace std*/;
