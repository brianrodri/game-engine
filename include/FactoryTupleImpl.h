struct offsetFoldFunctor {

    template <typename O, typename SzAl>
    constexpr auto operator()(O o, SzAl szalPair) const
    {
        using boost::hana::literals::operator""_c;
        return (o + szalPair[0_c] + szalPair[1_c] - boost::hana::size_c<1>) / szalPair[1_c] *
            szalPair[1_c];
    }

} /*struct offsetFoldFunctor*/;


struct offsetOfFunctor {

    template<typename Tup, std::size_t N = boost::hana::length(Tup{})>
    constexpr std::size_t operator()(Tup&& tup, boost::hana::size_t<N> n = {})
    {
        using namespace boost::hana;
        return fold(
            slice_c<1, N + 1>(
                zip(
                    prepend(transform(tup, sizeof_), size_c<0>)
                  , append(
                        transform(tup, alignof_)
                      , maximum(prepend(transform(tup, alignof_), size_c<1>))
                    )
                )
            )
          , size_c<0>
          , offsetFoldFunctor{}
        );
    }


} /*struct offsetOfFunctor*/;


class accessOneFunctor {

    Self * that;

public:

    constexpr accessOneFunctor(Self * thisIn) : that{thisIn} { }

    template <size_t I>
    constexpr auto& operator()(boost::hana::size_t<I> i) const
    {
        using U = std::tuple_element_t<I, std::tuple<T...>>;
        char * memberPtr = reinterpret_cast<char*>(&that->m_memory) + offsetOfFunctor{}(boost::hana::tuple_t<T...>, i);
        return *reinterpret_cast<U*>(memberPtr);
    }

} /*struct accessOneFunctor*/;


class refWrapOneFunctor {

    Self *  that;
    accessOneFunctor  accessor;

public:

    constexpr refWrapOneFunctor(Self * thisIn) : that{thisIn}, accessor{thisIn} { }

    template <size_t I>
    constexpr auto operator()(boost::hana::size_t<I> i) const
    {
        return std::ref(accessor(i));
    }

} /*class constRefWrapOneFunctor*/;


class constAccessOneFunctor {

    const Self *  that;

public:

    constexpr constAccessOneFunctor(const Self * thisIn) : that{thisIn} { }

    template <size_t I>
    constexpr const auto& operator()(boost::hana::size_t<I> i) const
    {
        using U = std::tuple_element_t<I, std::tuple<T...>>;
        const char * memberPtr = reinterpret_cast<const char*>(&that->m_memory) + offsetOfFunctor{}(boost::hana::tuple_t<T...>, i);
        return *reinterpret_cast<const U*>(memberPtr);
    }

} /*class constAccessOneFunctor*/;


class constRefWrapOneFunctor {

    const Self *  that;
    constAccessOneFunctor  accessor;

public:

    constexpr constRefWrapOneFunctor(const Self * thisIn) : that{thisIn}, accessor{thisIn} { }

    template <size_t I>
    constexpr auto operator()(boost::hana::size_t<I> i) const
    {
        return std::cref(accessor(i));
    }

} /*class constRefWrapOneFunctor*/;


class constructOneFunctor {

    Self * that;

public:

    constexpr constructOneFunctor(Self * thisIn) : that{thisIn} { }

    template<size_t I, typename... A>
    constexpr void operator()(boost::hana::size_t<I> i, A&&... args) const
    {
        using U = std::tuple_element_t<I, std::tuple<T...>>;
        static_assert(std::is_constructible<U, A...>::value);
        new (&(accessOneFunctor{that}(i))) U(std::forward<A>(args)...);
    }

} /*class constructOneFunctor*/;


class factoryConstructOneFunctor {

    Self * that;
    constructOneFunctor  ctor;

public:

    constexpr factoryConstructOneFunctor(Self * thisIn) : that{thisIn}, ctor{thisIn} { }

    template<size_t N, typename F>
    constexpr void operator()(boost::hana::size_t<N> n, F&& f)
    {
        boost::hana::unpack(f(*that), boost::hana::partial(ctor, n));
    }

} /*class factoryConstructOneFunctor*/;


class destructOneFunctor {

    Self * that;

public:

    constexpr destructOneFunctor(Self * thisIn) : that{thisIn} { }

    template<size_t I>
    constexpr void operator()(boost::hana::size_t<I> i) const
    {
        using U = std::tuple_element_t<I, std::tuple<T...>>;
        accessOneFunctor{that}(i).~U();
    }

} /*class destructOneFunctor*/;

class tupleCopyFunctor {

    const Self * that;

public:

    constexpr tupleCopyFunctor(const Self * thisIn) : that{thisIn} { }

    template <size_t... I>
    constexpr std::tuple<T...> operator()(std::index_sequence<I...>) const
    {
        return { T{that->operator[](boost::hana::size_c<I>)}... };
    }

} /*class tupleCopyFunctor*/;

class hanaTupleCopyFunctor {

    const Self * that;

public:

    constexpr hanaTupleCopyFunctor(const Self * thisIn) : that{thisIn} { }

    template <size_t... I>
    constexpr boost::hana::tuple<T...> operator()(std::index_sequence<I...>) const
    {
        return { T{that->operator[](boost::hana::size_c<I>)}... };
    }

} /*class tupleCopyFunctor*/;

