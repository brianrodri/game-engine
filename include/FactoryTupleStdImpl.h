namespace std {

template<typename... T>
class tuple_size<FactoryTuple<T...>> : public std::integral_constant<std::size_t, sizeof...(T)> {
};


template<size_t I, typename... T>
constexpr auto& get(FactoryTuple<T...>& o)
{
    return o[boost::hana::size_c<I>];
}


template<size_t I, typename... T>
constexpr const auto& get(const FactoryTuple<T...>& o)
{
    return o[boost::hana::size_c<I>];
}


template<typename U, typename... T>
constexpr auto& get(FactoryTuple<T...>& o)
{
    return o[boost::hana::size_c<U>];
}


template<typename U, typename... T>
constexpr const auto& get(const FactoryTuple<T...>& o)
{
    return o[boost::hana::size_c<U>];
}

} /*namespace std*/;
