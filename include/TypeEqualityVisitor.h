#pragma once
#include <aetee/type_c.h>

template <typename U>
struct TypeEqualityVisitor {

    template <typename T>
    constexpr bool operator()(aetee::type_constant_t<T>) const
    {
        return std::is_same<U, T>::value;
    }

} /*struct TypeEqualityVisitor*/;

