#include "FactoryTuple.h"
#include <string>
#include <tuple>
#include <utility>
#include <experimental/array>
#include <experimental/type_traits>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

using aetee::operator""_c;


TEST(FactoryTuple, EmptyTupleExists)
{
    FactoryTuple<> empty{};
    EXPECT_EQ(1, sizeof(empty));
}

TEST(FactoryTuple, DefaultProduction)
{
    auto expected = std::make_tuple(0, 0);
    FactoryTuple<int, int> actual{};
    EXPECT_EQ(2*sizeof(int), sizeof(FactoryTuple<int, int>));
    EXPECT_EQ(expected, (decltype(expected)) actual);
}

TEST(FactoryTuple, HomoProduction)
{
    auto expected = std::make_tuple(2, 1, 3);
    FactoryTuple<int, int, int> actual{
        [&](auto& e) { return std::make_tuple(2); }
      , [&](auto& e) { return std::make_tuple(1); }
      , [&](auto& e) { return std::make_tuple(3); }
        };
    EXPECT_EQ(expected, (decltype(expected)) actual);
}

TEST(FactoryTuple, HeteroProduction)
{
    auto expected = std::make_tuple(1, 3, 2);
    FactoryTuple<int, float, double> actual{
        [&](auto& e) { return std::make_tuple(1); }
      , [&](auto& e) { return std::make_tuple(3); }
      , [&](auto& e) { return std::make_tuple(2); }
        };
    EXPECT_EQ(expected, (decltype(expected)) actual);
}

TEST(FactoryTuple, HomoAssignment)
{
    auto expected = std::make_tuple(2, 1, 3);
    FactoryTuple<int, int, int> actual{
        [&](auto& e) { return std::tie(std::get<0>(expected)); }
      , [&](auto& e) { return std::tie(std::get<1>(expected)); }
      , [&](auto& e) { return std::tie(std::get<2>(expected)); }
        };
    EXPECT_EQ(expected, (decltype(expected)) actual);
}

TEST(FactoryTuple, HeteroAssignment)
{
    auto expected = std::make_tuple(1, 3, 2);
    FactoryTuple<int, float, double> actual{
        [&](auto& e) { return std::tie(std::get<0>(expected)); }
      , [&](auto& e) { return std::tie(std::get<1>(expected)); }
      , [&](auto& e) { return std::tie(std::get<2>(expected)); }
        };
    EXPECT_EQ(expected, (decltype(expected)) actual);
}

TEST(FactoryTuple, PaddedAssignment)
{
    auto expected = std::make_tuple(1, 'a', 3, 'b', 'c', 2, 0);
    FactoryTuple<int, char, float, char, char, double, int> actual{
        [&](auto& e) { return std::tie(std::get<0>(expected)); }
      , [&](auto& e) { return std::tie(std::get<1>(expected)); }
      , [&](auto& e) { return std::tie(std::get<2>(expected)); }
      , [&](auto& e) { return std::tie(std::get<3>(expected)); }
      , [&](auto& e) { return std::tie(std::get<4>(expected)); }
      , [&](auto& e) { return std::tie(std::get<5>(expected)); }
      , [&](auto& e) { return std::tie(std::get<6>(expected)); }
        };
    EXPECT_EQ(expected, (decltype(expected)) actual);
}

TEST(FactoryTuple, DependantProduction)
{
    auto expected = std::tuple<std::string, std::string>{"down the pipe", "out the pipe"};
    auto simple = [](auto& _) { return std::make_tuple("down the pipe"); };
    auto dependant = [](auto& e) {
        std::string bottle{e[0_c]};
        bottle.replace(0, 4, "out");
        return std::make_tuple(std::move(bottle));
    };
    FactoryTuple<std::string, std::string> actual{simple, dependant};
    EXPECT_EQ(expected, (decltype(expected)) actual);
}
