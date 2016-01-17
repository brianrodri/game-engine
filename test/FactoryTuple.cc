#include "FactoryTuple.h"
#include "my_literals.h"
#include <string>
#include <tuple>
#include <utility>
#include <experimental/array>
#include <gtest/gtest.h>
#include <gmock/gmock.h>



TEST(FactoryTuple, EmptyTuple)
{
    FactoryTuple<> empty{};
    EXPECT_EQ(1, sizeof(empty));
}

TEST(FactoryTuple, DefaultConstructs)
{
    std::tuple<int, int> expected{0, 0};
    FactoryTuple<int, int> actual{};
    EXPECT_EQ(2*sizeof(int), sizeof(FactoryTuple<int, int>));
    EXPECT_EQ(expected, actual.to_tuple());
}

TEST(FactoryTuple, HomogeneousAssignment)
{
    std::tuple<int, int, int> expected{2, 1, 3};
    FactoryTuple<int, int, int> actual{
        [&](auto& e) { return std::forward_as_tuple(std::get<0>(expected)); }
      , [&](auto& e) { return std::forward_as_tuple(std::get<1>(expected)); }
      , [&](auto& e) { return std::forward_as_tuple(std::get<2>(expected)); }
        };
    EXPECT_EQ(expected, actual.to_tuple());
}

TEST(FactoryTuple, HeterogeneousAssignment)
{
    std::tuple<int, double, long double> expected{1, 3.14, 2.71828};
    FactoryTuple<int, double, long double> actual{
        [&](auto& e) { return std::forward_as_tuple(std::get<0>(expected)); }
      , [&](auto& e) { return std::forward_as_tuple(std::get<1>(expected)); }
      , [&](auto& e) { return std::forward_as_tuple(std::get<2>(expected)); }
        };
    EXPECT_EQ(expected, actual.to_tuple());
}

/*
TEST(FactoryTuple, HomogeneousProduction)
{
    std::tuple<int, int, int> expected{2, 1, 3};
    FactoryTuple<int, int, int> actual{
        [&](auto& e) { return std::forward_as_tuple(2); }
      , [&](auto& e) { return std::forward_as_tuple(1); }
      , [&](auto& e) { return std::forward_as_tuple(3); }
        };
    EXPECT_EQ(expected, actual.to_tuple());
}

TEST(FactoryTuple, HeterogeneousProduction)
{
    std::tuple<int, double, long double> expected{1, 3.14, 2.71828};
    FactoryTuple<int, double, long double> actual{
        [&](auto& e) { return std::forward_as_tuple(1); }
      , [&](auto& e) { return std::forward_as_tuple(3.14); }
      , [&](auto& e) { return std::forward_as_tuple(2.71828); }
        };
    EXPECT_EQ(expected, actual.to_tuple());
}

TEST(FactoryTuple, BasicFactoryConstructs)
{
    using std::literals::string_literals::operator""s;
    auto makefoo = [](auto&) { return std::forward_as_tuple("foo"); };
    auto makebar = [](auto&) { return std::forward_as_tuple("bar"); };

    FactoryTuple<std::string, std::string> strpair{makefoo, makebar};

    EXPECT_EQ(strpair[0_c], "foo");
    EXPECT_EQ(strpair[1_c], "bar");
}

TEST(FactoryTuple, ComplexFactoryConstructs)
{
    auto simple = [](auto& _) {
        return std::forward_as_tuple("down the pipe!");
    };
    auto dependant = [](auto& e) {
        std::string bottle{e[0_c]};
        bottle.replace(0, 4, "out");
        return std::forward_as_tuple(std::move(bottle));
    };

    FactoryTuple<std::string, std::string> strpair{simple, dependant};

    EXPECT_EQ(strpair[0_c], "down the pipe!");
    EXPECT_EQ(strpair[1_c], "out the pipe!");
}
*/
