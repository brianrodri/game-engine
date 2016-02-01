/*
#include <string>
#include <experimental/tuple>
#include <functional>
#include "test.h"
#include <aetee/aetee.h>
#include <aetee/prettyprint.h>

TEST(AeteeTypeConstant)
{
    auto int_c = aetee::type_c<int>;
    auto string_c = aetee::type_c<std::string>;
    auto isPOD = [](auto t) { return std::is_pod<typename decltype(t)::type>::value; };
    bool int_is_pod = isPOD(int_c);
    bool string_is_pod = isPOD(string_c);
    CHECK_EQUAL(int_is_pod, string_is_pod);
}

TEST(AeteeAppend)
{
    using aetee::operator<<;
    auto expected = std::make_tuple(1, 2, 3);
    auto actual = aetee::append(std::make_tuple(1, 2), 3);
    CHECK_EQUAL(expected, actual);
}

TEST(AeteePrepend)
{
    auto expected = std::make_tuple(1, 2, 3);
    auto actual = aetee::prepend(std::make_tuple(2, 3), 1);
    CHECK_EQUAL(expected, actual);
}

TEST(AeteeFold)
{
    auto expected = 10;
    auto actual = aetee::fold(std::make_tuple(1, 2, 3, 4), 0, std::plus<>{});
    CHECK_EQUAL(expected, actual);
}
*/
