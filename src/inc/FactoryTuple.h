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
    using Byte = unsigned char;
    using Self = FactoryTuple<T...>;
    using Tuple = std::tuple<T...>;
    static constexpr auto alignForAll = aetee::alignofall(aetee::type_c<T>...);

    //! Calculates offset to `type_at<I, T...>` based on (sizeof, alignof)(T...)
    struct AccessCalculator {
        template <size_t I>
        constexpr auto operator()(aetee::index_constant_t<I> l) const
        {
            using namespace aetee;
            constexpr std::array<size_t, arity_c<T...> + 1> szs{0            , sizeof(T)...};
            constexpr std::array<size_t, arity_c<T...> + 1> als{alignof(T)..., alignForAll};
            size_t result{};
            for (size_t i{0}; i < I + 1; ++i) {
                result = ((result + szs[i] + als[i] - 1) / als[i]) * als[i];
            }
            return result;
        }
    } /*struct AccessFunctor*/;

    struct ReferenceTyingFunctor {
        template <typename... U, size_t... I>
        constexpr auto operator()(const Self& tup, aetee::index_sequence_t<I...>) const
        {
            return std::tie(tup[aetee::index_c<I>]...);
        }
    } /*struct ReferenceTyingFunctor*/;

    //! Creates references from a base pointer to offset of `type_at<I, T...>`
    struct AccessGenerator {
        template <size_t I>
        constexpr auto& operator()(Byte * base_ptr, aetee::index_constant_t<I> tupIdx) const
        {
            constexpr size_t offset = AccessCalculator{}(tupIdx);
            return *reinterpret_cast<aetee::type_at_t<I, T...> *>(
                reinterpret_cast<Byte *>(base_ptr) + offset
                );
        }
        template <size_t I>
        constexpr const auto& operator()(const Byte * base_ptr, aetee::index_constant_t<I> tupIdx) const
        {
            constexpr size_t offset = AccessCalculator{}(tupIdx);
            return *reinterpret_cast<const aetee::type_at_t<I, T...> *>(
                reinterpret_cast<const Byte *>(base_ptr) + offset
                );
        }
    } /*struct AccessGenerator*/;

    struct ConstructionFunctor {
        template <typename U, typename... A>
        constexpr void operator()(U* ptr, A&&... args) const
        {
            new(ptr) U{std::forward<A>(args)...};
        }
    } /*struct ConstructionFunctor*/;

    struct DefaultConstructionLogic {
        template <size_t I>
        constexpr void operator()(Self& tup, aetee::index_constant_t<I> i) const
        {
            ConstructionFunctor{}(&tup[i]);
        }
    } /*struct DefaultConstructionLogic*/;

    struct FactoryConstructionLogic {
        template <size_t I, typename F>
        constexpr void operator()(Self& tup, aetee::index_constant_t<I> i, F&& f) const
        {
            constexpr auto ctor = aetee::curry(ConstructionFunctor{}, &tup[i]);
            aetee::apply(f(tup), ctor);
        }
    } /*struct FactoryConstructionLogic*/;

    struct DestructionLogic {
        template <size_t I>
        constexpr void operator()(Self& tup, aetee::index_constant_t<I> i) const
        {
            using U = aetee::type_at_t<I, T...>;
            tup[i].~U();
        }
    } /*struct DestructionLogic*/;

public:
    //! Default Ctor
    /**
     * Only exists when all `T`s are default constructible.
     */
    constexpr FactoryTuple()
    {
        using namespace aetee;
        for_each(
            zip(
                replicate(std::ref(*this), arity_c<T...>)
              , index_sequence_c_for<T...>
                )
          , apply.wrapper(DefaultConstructionLogic{})
            );
    }

    //! Factory Ctor
    template <typename... F>
    constexpr FactoryTuple(F&&... f)
    {
        using namespace aetee;
        for_each(
            zip(
                replicate(std::ref(*this), arity_c<T...>)
              , index_sequence_c_for<T...>
              , std::forward_as_tuple(std::forward<F>(f)...)
                )
          , apply.wrapper(FactoryConstructionLogic{})
            );
    }

    //! Destructor
    ~FactoryTuple() 
    {
        using namespace aetee;
        for_each(
            zip(
                replicate(std::ref(*this), arity_c<T...>)
              , reverse(index_sequence_c_for<T...>)
                )
          , apply.wrapper(DestructionLogic{})
            );
    }

    //! FactoryTuple must remain in-place to maintain valid references
    FactoryTuple(Self const&) = delete;
    FactoryTuple(Self&&) = delete;
    Self& operator=(Self const&) = delete;
    Self& operator=(Self&&) = delete;

    //! Allows an explicit cast to a `std::tuple<U...>`
    /**
     * I made this to make testing easier mostly, but also because I feel this is the
     * most expressive way of saying "treat this is a tuple," and it keeps well with
     * comparisons to things
     */
    template <typename... U>
    constexpr explicit operator std::tuple<U...>() const
    {
        return ReferenceTyingFunctor{}(std::cref(*this), aetee::index_sequence_c_for<U...>);
    }

    //! Allows compile-time access through an index into the typelist
    template <size_t I>
    constexpr auto& operator[](aetee::index_constant_t<I> i) 
    {
        return AccessGenerator{}(m_memory.begin(), i);
    }

    //! Allows constant compile-time access through an index into the typelist
    template <size_t I>
    constexpr const auto& operator[](aetee::index_constant_t<I> i) const 
    {
        return AccessGenerator{}(m_memory.cbegin(), i);
    }

    //! Allows compile-time access through an element of the typelist
    template <typename U>
    constexpr auto& operator[](aetee::type_constant_t<U> t) 
    {
        return AccessGenerator{}(m_memory.begin(), aetee::type_index_c<U, T...>);
    }

    //! Allows constant compile-time access through an element of the typelist
    template <typename U>
    constexpr const auto& operator[](aetee::type_constant_t<U> t) const 
    {
        return AccessGenerator{}(m_memory.cbegin(), aetee::type_index_c<U, T...>);
    }

private:
    alignas(alignForAll) std::array<Byte, AccessCalculator{}(aetee::arity_c<T...>)> m_memory;
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
    static constexpr auto value = aetee::index_c<sizeof...(T)>;
} /*struct tuple_size*/;

} /*namespace std*/;
