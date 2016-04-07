#pragma once
#include <functional>
#include <memory>
#include <tuple>
#include <utility>
#include <aetee/int_c.h>
#include <aetee/traits/alignof.h>
#include <aetee/traits/offsetof.h>
#include <aetee/axioms/reverse.h>

/**
 * @brief   A `FactoryTuple` is a variant of `std::tuple` which allows its
 *          members to access earlier members during construction.  
 *
 * @tparam  `T...` types that the tuple will attempt to construct
 *
 *  This allows the creation of complex types that maintain close spatial
 *  locality with the types they depend upon.  To access the members of the
 *  struct, FactoryTuple is constructed with **factory functions**, which take
 *  as an argument the FactoryTuple itself and return the arguments required to
 *  construct its correspondant element *as a tuple*.
 *
 *  N.B.:  It is undefined behavior to access and use elements that have yet to
 *  be constructed!
 *
 *  Code is clearer than words, so here is some code that demonstrates its
 *  behavior:
 *
 *  ```cpp
 *  FactoryTuple<int, char, std::string> sampleTuple
 *    { [](auto& underConstructionTup) { return std::make_tuple(5); }
 *    , [](auto& underConstructionTup) { return std::make_tuple('c'); }
 *    , [](auto& underConstructionTup) {
 *          return std::make_tuple(
 *              std::string
 *                { std::get<0>(underConstructionTup)
 *                , std::get<1>(underConstructionTup)
 *              }
 *          );
 *      }
 *  };
 *
 *  std::cout << std::get<2>(sampleTuple) << '\n';
 *  ```
 *  Outputs: ccccc
 *
 *      The memory layout of the `std::tuple` follows struct-like memory
 *  offsets.  More mathematically, the memory offset of each member of the
 *  struct follows the following recurrence relation:
 *
 *  offset[0] = 0
 *  offset[i] = (offset[i-1] + sizeof[i-1] + alignof[i] - 1) / alignof[i] * alignof[i]
 *
 *      Since references are so important to the concept of a FactoryTuple, I've
 *  made the design decision to *delete copy and move semantics*.  This way,
 *  there is never any need to worry about the invalidation of references.
 *
 *      I may consider adding a function `rebind` which gives the programmer a
 *  chance to reset the references that each member requires.  Until then, a
 *  FactoryTuple must stay rooted in whatever memory location it was constructed
 *  upon.
 */
template<typename... T>
class FactoryTuple {

    using Self = FactoryTuple<T...>;
    static constexpr const auto Len = aetee::offsetof_(aetee::type_c_sequence<T...>);
    static constexpr const auto Align = aetee::alignof_(aetee::type_c_sequence<T...>);

public:

    /**
     * @brief  Default constructs each of the members `T...`
     *
     *  WARNING:  This command will only exist when each `T...` can be *default
     *  constructed*!
     */
    constexpr FactoryTuple()
    {
        defaultConstruct(aetee::idx_c_sequence_for<T...>);
    }

    /**
     * @brief Initializes each `T...` through the tuple returned by `F(*this)`
     *
     * @tparam  F...     A series of functions that accept *this as an argument
     *                   and return the required types to properly construct its
     *                   correspondant `T...`
     * @param fs
     *
     *      Initializes each `T` through the tuple returned by a call to
     *  f(*this).  The tuple may store references to earlier-constructed `T`s if
     *  so desired, allowing complex dependencies to live next to each other in
     *  memory through a single instance of FactoryTuple.
     *
     *      Each `T...` is constructed **in the same order as the type list**.
     */
    template<typename... F>
    constexpr FactoryTuple(F&&... fs)
    {
        static_assert(aetee::arity_c<F...> == aetee::arity_c<T...>);
        factoryConstruct(aetee::idx_c_sequence_for<T...>, std::forward<F>(fs)...);
    }

    //! Each `T...` is destructed **in the reverse order of their listing**.
    ~FactoryTuple() 
    {
        destruct(aetee::reverse(aetee::idx_c_sequence_for<T...>));
    }

    // FactoryTuple must remain in-place to maintain valid references
    FactoryTuple(Self const&) = delete;
    FactoryTuple(Self&&) = delete;
    Self& operator=(Self const&) = delete;
    Self& operator=(Self&&) = delete;

    /**
     * @brief   Grants compile-time access to a member through the aetee idx_c
     *
     * @tparam  I   The index into the tuple to be chosen.
     *
     * @return  The member of type `T...`[i] at offset[i].
     */
    template<size_t I>
    constexpr auto& operator[](aetee::idx_t<I> i) 
    {
        using U = aetee::type_at_t<I, T...>;
        char * memberPtr = reinterpret_cast<char*>(&m_memory) + aetee::offsetof_(aetee::type_c_sequence<T...>, i);
        return *reinterpret_cast<U*>(memberPtr);
    }

    /**
     * @brief   Grants const compile-time access to a member through the aetee
     *          idx_c
     *
     * @tparam  I   The index into the tuple to be chosen.
     *
     * @return  The member of type `T...`[i] at offset[i].
     */
    template<size_t I>
    constexpr const auto& operator[](aetee::idx_t<I> i) const 
    {
        using U = aetee::type_at_t<I, T...>;
        const char * memberPtr = reinterpret_cast<const char*>(&m_memory) + aetee::offsetof_(aetee::type_c_sequence<T...>, i);
        return *reinterpret_cast<const U*>(memberPtr);
    }

    /**
     * @brief   Grants compile-time access to a member through a direct type
     *
     * @tparam  U   The type to be fetched from the tuple
     *
     * @return  The first member of type U in the list `T...`
     */
    template<typename U>
    constexpr auto& operator[](aetee::type_t<U> t) 
    {
        return operator[](aetee::type_idx_c<U, T...>);
    }

    /**
     * @brief   Grants const compile-time access to a member through a direct
     *          type
     *
     * @tparam  U   The type to be fetched from the tuple
     *
     * @return  The first member of type U in the list `T...`
     */
    template<typename U>
    constexpr const auto& operator[](aetee::type_t<U> t) const 
    {
        return operator[](aetee::type_idx_c<U, T...>);
    }

    //! Returns a tuple containing references to each member of this
    constexpr auto tie()
    {
        return tieImpl(aetee::idx_c_sequence_for<T...>);
    }

    //! Returns a tuple containing const references to each member of this
    constexpr auto tie() const
    {
        return tieImpl(aetee::idx_c_sequence_for<T...>);
    }

private:

    template<size_t I, typename... A>
    constexpr void constructOne(aetee::idx_t<I> i, A&&... args)
    {
        using U = aetee::type_at_t<I, T...>;
        static_assert(std::is_constructible<U, A...>::value);
        new(&operator[](i)) U(std::forward<A>(args)...);
    }

    template<size_t I>
    constexpr void destructOne(aetee::idx_t<I> i)
    {
        using U = aetee::type_at_t<I, T...>;
        operator[](i).~U();
    }

    template<size_t... I>
    constexpr void defaultConstruct(aetee::idx_c_sequence_t<I...>)
    {
        (constructOne(aetee::idx_c<I>), ...);
    }

    template<size_t... I, typename... F>
    constexpr void factoryConstruct(aetee::idx_c_sequence_t<I...>, F&&... f)
    {
        (factoryConstructOne(aetee::idx_c<I>, aetee::idx_c_sequence_of<std::result_of_t<F(Self&)>>, f(*this)), ...);
    }

    template<size_t I, size_t... J, typename Yield>
    constexpr void factoryConstructOne(aetee::idx_t<I> i, aetee::idx_c_sequence_t<J...> js, Yield&& y)
    {
        constructOne(i, std::get<J>(std::forward<Yield>(y))...);
    }

    template<size_t... I>
    constexpr void destruct(aetee::idx_c_sequence_t<I...>)
    {
        (destructOne(aetee::idx_c<I>), ...);
    }

    template<size_t... I>
    constexpr auto tieImpl(aetee::idx_c_sequence_t<I...>)
    {
        return std::tie(operator[](aetee::idx_c<I>)...);
    }

    template<size_t... I>
    constexpr auto tieImpl(aetee::idx_c_sequence_t<I...>) const
    {
        return std::tie(operator[](aetee::idx_c<I>)...);
    }

    std::aligned_storage_t<Len, Align>  m_memory;

} /*class FactoryTuple*/;

namespace std {

template<typename... T>
class tuple_size<FactoryTuple<T...>> : public std::integral_constant<std::size_t, sizeof...(T)> { };

template<size_t I, typename... T>
    constexpr auto& get(FactoryTuple<T...>& o) { return o[aetee::idx_c<I>]; }
template<size_t I, typename... T>
    constexpr const auto& get(const FactoryTuple<T...>& o) { return o[aetee::idx_c<I>]; }
template<typename U, typename... T>
    constexpr auto& get(FactoryTuple<T...>& o) { return o[aetee::type_c<U>]; }
template<typename U, typename... T>
    constexpr const auto& get(const FactoryTuple<T...>& o) { return o[aetee::type_c<U>]; }

} /*namespace std*/;
