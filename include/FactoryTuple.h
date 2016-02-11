#pragma once
#include <functional>
#include <memory>
#include <utility>
#include <experimental/array>
#include <experimental/type_traits>
#include <experimental/tuple>
#include <aetee/aetee.h>

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
    static constexpr const size_t Len = aetee::offsetof_(aetee::type_sequence_c<T...>);
    static constexpr const size_t Align = aetee::alignof_(aetee::type_sequence_c<T...>);
    template <size_t I> using TypeAt = aetee::type_at_t<I, T...>;

public:
    //! Default Ctor
    /**
     * Only exists when all `T`s are default constructible.
     */
    constexpr FactoryTuple()
    {
        defaultConstruct(aetee::index_sequence_c_for<T...>);
    }

    //! Factory Ctor
    template <typename... F>
    constexpr FactoryTuple(F&&... f)
    {
        static_assert(aetee::arity_c<F...> == aetee::arity_c<T...>);
        factoryConstruct(aetee::index_sequence_c_for<T...>, std::forward<F>(f)...);
    }

    //! Destructor
    ~FactoryTuple() 
    {
        destruct(aetee::reverse(aetee::index_sequence_c_for<T...>));
    }

    //! FactoryTuple must remain in-place to maintain valid references
    FactoryTuple(Self const&) = delete;
    FactoryTuple(Self&&) = delete;
    Self& operator=(Self const&) = delete;
    Self& operator=(Self&&) = delete;

    //! Allows compile-time access through an index into the typelist
    template <size_t I>
    constexpr auto& operator[](aetee::index_constant_t<I> i) 
    {
        char * raw = reinterpret_cast<char*>(&m_memory) + aetee::offsetof_(aetee::type_sequence_c<T...>, i);
        return *reinterpret_cast<TypeAt<I>*>(raw);
    }

    //! Allows constant compile-time access through an index into the typelist
    template <size_t I>
    constexpr const auto& operator[](aetee::index_constant_t<I> i) const 
    {
        const char * raw = reinterpret_cast<const char*>(&m_memory) + aetee::offsetof_(aetee::type_sequence_c<T...>, i);
        return *reinterpret_cast<const TypeAt<I>*>(raw);
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

    //! Returns a tuple containing references to each member of this
    constexpr auto as_tuple()
    {
        return tie(aetee::index_sequence_c_for<T...>);
    }

    //! Returns a tuple containing const references to each member of this
    constexpr auto as_tuple() const
    {
        return tie(aetee::index_sequence_c_for<T...>);
    }

private:
    template <size_t I, typename... A>
    constexpr void explicitConstruct(aetee::index_constant_t<I> i, A&&... args)
    {
        new(&operator[](i)) TypeAt<I>(std::forward<A>(args)...);
    }

    template <size_t... I>
    constexpr void defaultConstruct(aetee::index_sequence_t<I...>)
    {
        (explicitConstruct(aetee::index_c<I>), ...);
    }

    template <size_t... I, typename... F>
    constexpr void factoryConstruct(aetee::index_sequence_t<I...>, F&&... f)
    {
        (factoryConstruct(aetee::index_c<I>, aetee::indices_of_c<std::result_of_t<F(Self&)>>, f(*this)), ...);
    }

    template <size_t I, size_t... J, typename Yield>
    constexpr void factoryConstruct(aetee::index_constant_t<I> i, aetee::index_sequence_t<J...> js, Yield&& y)
    {
        explicitConstruct(i, std::get<J>(std::forward<Yield>(y))...);
    }

    template <size_t... I>
    constexpr void destruct(aetee::index_sequence_t<I...>)
    {
        (operator[](aetee::index_c<I>).~TypeAt<I>(), ...);
    }

    template <size_t... I>
    constexpr auto tie(aetee::index_sequence_t<I...>)
    {
        return std::tie(operator[](aetee::index_c<I>)...);
    }

    template <size_t... I>
    constexpr auto tie(aetee::index_sequence_t<I...>) const
    {
        return std::tie(operator[](aetee::index_c<I>)...);
    }

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
