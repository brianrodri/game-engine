#pragma once
#include <algorithm>
#include <array>
#include <functional>
#include <memory>
#include <tuple>
#include <utility>
#include <boost/hana.hpp>
#include <boost/hana/ext/std/tuple.hpp>

namespace hana = boost::hana;
using hana::literals::operator""_c;

namespace detail {

}

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

    #define idx_tuple_c (hana::to_tuple(hana::range_c<std::size_t, 0, sizeof...(T)>))
    #define type_tuple_c (hana::tuple_t<T...>)
    #define typeIdx_map_c (hana::unpack(hana::zip_with(hana::make_pair, type_tuple_c, idx_tuple_c), hana::make_map))

    //! Forward declarations of implementation details
    struct accessOneFunctor;
    struct constAccessOneFunctor;
    struct constructOneFunctor;
    struct destructOneFunctor;
    struct factoryConstructOneFunctor;
    struct offsetOfFunctor;

    using Self = FactoryTuple<T...>;

public:

    /**
     * @brief  Default constructs each of the members `T...`
     *
     *  WARNING:  This command will only exist when each `T...` can be *default
     *  constructed*!
     */
    constexpr FactoryTuple()
    {
        hana::for_each(idx_tuple_c, constructOneFunctor{this});
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
        hana::for_each(
            hana::zip(idx_tuple_c, hana::make_tuple(std::forward<F>(fs)...))
          , hana::fuse(factoryConstructOneFunctor{this})
        );
    }

    //! Each `T...` is destructed **in the reverse order of their listing**.
    ~FactoryTuple() 
    {
        hana::for_each(hana::reverse(idx_tuple_c), destructOneFunctor{this});
    }

    // FactoryTuple must remain in-place to maintain valid references
    FactoryTuple(Self const&) = delete;
    FactoryTuple(Self&&) = delete;
    Self& operator=(Self const&) = delete;
    Self& operator=(Self&&) = delete;

    /**
     * @brief   Grants compile-time access to a member through the size_c
     *
     * @tparam  I   The index into the tuple to be chosen.
     *
     * @return  The member of type `T...`[i] at offset[i].
     */
    template<typename X, X I>
    constexpr auto& operator[](hana::integral_constant<X, I>)
    {
        return accessOneFunctor{this}(hana::size_c<I>);
    }

    /**
     * @brief   Grants const compile-time access to a member through the aetee
     *          idx_c
     *
     * @tparam  I   The index into the tuple to be chosen.
     *
     * @return  The member of type `T...`[i] at offset[i].
     */
    template<typename X, X I>
    constexpr const auto& operator[](hana::integral_constant<X, I>) const 
    {
        return constAccessOneFunctor{this}(hana::size_c<I>);
    }

    /**
     * @brief   Grants compile-time access to a member through a direct type
     *
     * @tparam  U   The type to be fetched from the tuple
     *
     * @return  The first member of type U in the list `T...`
     */
    template<typename U>
    constexpr auto& operator[](hana::type<U> t) 
    {
        return accessOneFunctor{this}(typeIdx_map_c[t]);
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
    constexpr const auto& operator[](hana::type<U> t) const 
    {
        return constAccessOneFunctor{this}(typeIdx_map_c[t]);
    }

    //! Returns a tuple containing references to each member of this
    constexpr auto tie()
    {
        return hana::transform(idx_tuple_c, refWrapOneFunctor{this});
    }

    //! Returns a tuple containing const references to each member of this
    constexpr auto ctie() const
    {
        return hana::transform(idx_tuple_c, constRefWrapOneFunctor{this});
    }

    //! Returns a copy of the tuple
    constexpr auto to_tuple() const
    {
        return hana::tuple<T...>{this->ctie()};
    }

private:

    #include "FactoryTupleImpl.h"

    static constexpr const std::size_t Len = offsetOfFunctor{}(hana::tuple_t<T...>);
    static constexpr const std::size_t Align = std::max({std::size_t{1}, alignof(T)...});
    std::aligned_storage_t<Len, Align>  m_memory;

} /*class FactoryTuple*/;

namespace std {

template<typename... T>
class tuple_size<FactoryTuple<T...>> : public std::integral_constant<std::size_t, sizeof...(T)> { };

template<size_t I, typename... T>
    constexpr auto& get(FactoryTuple<T...>& o) { return o[hana::size_c<I>]; }
template<size_t I, typename... T>
    constexpr const auto& get(const FactoryTuple<T...>& o) { return o[hana::size_c<I>]; }
template<typename U, typename... T>
    constexpr auto& get(FactoryTuple<T...>& o) { return o[hana::size_c<U>]; }
template<typename U, typename... T>
    constexpr const auto& get(const FactoryTuple<T...>& o) { return o[hana::size_c<U>]; }

} /*namespace std*/;
