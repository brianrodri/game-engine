#pragma once
#include <functional>
#include <memory>
#include <tuple>
#include <utility>
#include <aetee/int_c.h>
#include <aetee/traits/alignof.h>
#include <aetee/traits/offsetof.h>
#include <aetee/axioms/reverse.h>

//! Allows construction of elements through callables that take a ref to `this`
/**
 *      A `FactoryTuple` is a special type of tuple that allows its members to
 *  access each other for aid in their construction.  This allows the creation
 *  of tuples of complex types w/ their dependencies kept close together
 *  in-memory, and with minimal memory hoop-jumping as possible.
 */
template <typename... T>
class FactoryTuple {

    using Self = FactoryTuple<T...>;
    static constexpr const auto Len = aetee::offsetof_(aetee::type_c_sequence<T...>);
    static constexpr const auto Align = aetee::alignof_(aetee::type_c_sequence<T...>);

public:

    //! Default constructor
    /**
     * Only exists when all `T`s are default constructible.
     */
    constexpr FactoryTuple()
    {
        defaultConstruct(aetee::idx_c_sequence_for<T...>);
    }

    //! Factory constructor
    /**
     * Initializes each `T` through the tuple returned by a call to f(*this).
     * The tuple may store references to earlier-constructed `T`s if so
     * desired, allowing complex dependencies to live next to each other in
     * memory through a single instance of FactoryTuple.
     */
    template <typename... F>
    constexpr FactoryTuple(F&&... f)
    {
        static_assert(aetee::arity_c<F...> == aetee::arity_c<T...>);
        factoryConstruct(aetee::idx_c_sequence_for<T...>, std::forward<F>(f)...);
    }

    //! Destructor
    ~FactoryTuple() 
    {
        destruct(aetee::reverse(aetee::idx_c_sequence_for<T...>));
    }

    //! FactoryTuple must remain in-place to maintain valid references
    FactoryTuple(Self const&) = delete;
    FactoryTuple(Self&&) = delete;
    Self& operator=(Self const&) = delete;
    Self& operator=(Self&&) = delete;

    //! Allows compile-time access through an index into the typelist
    template <size_t I>
    constexpr auto& operator[](aetee::idx_constant_t<I> i) 
    {
        using U = aetee::type_at_t<I, T...>;
        char * memberPtr = reinterpret_cast<char*>(&m_memory) + aetee::offsetof_(aetee::type_c_sequence<T...>, i);
        return *reinterpret_cast<U*>(memberPtr);
    }

    //! Allows constant compile-time access through an index into the typelist
    template <size_t I>
    constexpr const auto& operator[](aetee::idx_constant_t<I> i) const 
    {
        using U = aetee::type_at_t<I, T...>;
        const char * memberPtr = reinterpret_cast<const char*>(&m_memory) + aetee::offsetof_(aetee::type_c_sequence<T...>, i);
        return *reinterpret_cast<const U*>(memberPtr);
    }

    //! Allows compile-time access through an element of the typelist
    template <typename U>
    constexpr auto& operator[](aetee::type_constant_t<U> t) 
    {
        return operator[](aetee::type_idx_c<U, T...>);
    }

    //! Allows constant compile-time access through an element of the typelist
    template <typename U>
    constexpr const auto& operator[](aetee::type_constant_t<U> t) const 
    {
        return operator[](aetee::type_idx_c<U, T...>);
    }

    //! Returns a tuple containing references to each member of this
    constexpr auto as_tuple()
    {
        return tie(aetee::idx_c_sequence_for<T...>);
    }

    //! Returns a tuple containing const references to each member of this
    constexpr auto as_tuple() const
    {
        return tie(aetee::idx_c_sequence_for<T...>);
    }

private:

    template <size_t I, typename... A>
    constexpr void constructOne(aetee::idx_constant_t<I> i, A&&... args)
    {
        using U = aetee::type_at_t<I, T...>;
        static_assert(std::is_constructible<U, A...>::value);
        new(&operator[](i)) U(std::forward<A>(args)...);
    }

    template <size_t I>
    constexpr void destructOne(aetee::idx_constant_t<I> i)
    {
        using U = aetee::type_at_t<I, T...>;
        operator[](i).~U();
    }

    template <size_t... I>
    constexpr void defaultConstruct(aetee::idx_sequence_t<I...>)
    {
        (constructOne(aetee::idx_c<I>), ...);
    }

    template <size_t... I, typename... F>
    constexpr void factoryConstruct(aetee::idx_sequence_t<I...>, F&&... f)
    {
        (factoryConstructOne(aetee::idx_c<I>, aetee::idx_c_sequence_of<std::result_of_t<F(Self&)>>, f(*this)), ...);
    }

    template <size_t I, size_t... J, typename Yield>
    constexpr void factoryConstructOne(aetee::idx_constant_t<I> i, aetee::idx_sequence_t<J...> js, Yield&& y)
    {
        constructOne(i, std::get<J>(std::forward<Yield>(y))...);
    }

    template <size_t... I>
    constexpr void destruct(aetee::idx_sequence_t<I...>)
    {
        (destructOne(aetee::idx_c<I>), ...);
    }

    template <size_t... I>
    constexpr auto tie(aetee::idx_sequence_t<I...>)
    {
        return std::tie(operator[](aetee::idx_c<I>)...);
    }

    template <size_t... I>
    constexpr auto tie(aetee::idx_sequence_t<I...>) const
    {
        return std::tie(operator[](aetee::idx_c<I>)...);
    }

    std::aligned_storage_t<Len, Align>  m_memory;

} /*class FactoryTuple*/;

namespace std {

template <typename... T>
class tuple_size<FactoryTuple<T...>> : public std::integral_constant<size_t, sizeof...(T)> {};

template <size_t I, typename... T>
    constexpr auto& get(FactoryTuple<T...>& o) { return o[aetee::idx_c<I>]; }
template <size_t I, typename... T>
    constexpr const auto& get(const FactoryTuple<T...>& o) { return o[aetee::idx_c<I>]; }
template <typename U, typename... T>
    constexpr auto& get(FactoryTuple<T...>& o) { return o[aetee::type_c<U>]; }
template <typename U, typename... T>
    constexpr const auto& get(const FactoryTuple<T...>& o) { return o[aetee::type_c<U>]; }

} /*namespace std*/;
