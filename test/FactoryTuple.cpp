#include "FactoryTuple.h"
#include <string>
#include <tuple>
#include <utility>
#include <experimental/array>
#include <experimental/type_traits>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <boost/hana.hpp>

namespace hana = boost::hana;
using hana::literals::operator""_c;
using namespace std::literals::string_literals;


template <typename... T, typename R>
constexpr bool operator==(const FactoryTuple<T...>& lhs, const R& rhs)
    { return lhs.to_tuple() == rhs; }
template <typename L, typename... T>
constexpr bool operator==(const L& lhs, const FactoryTuple<T...>& rhs)
    { return lhs == rhs.to_tuple(); }


TEST(FactoryTuple, EmptyTupleExists)
{
    FactoryTuple<> empty{};
    EXPECT_EQ(1, sizeof(empty));
}

TEST(FactoryTuple, DefaultProduction)
{
    std::tuple<int, int> expected{};
    FactoryTuple<int, int> actual{};
    EXPECT_EQ(2*sizeof(int), sizeof(actual));
    EXPECT_EQ(expected, actual.to_tuple());
}

TEST(FactoryTuple, HomogeneousProduction)
{
    std::tuple<int, int, int> expected
      { 2, 1, 3 };
    FactoryTuple<int, int, int> actual
      { [&](auto& e) { return std::make_tuple(2); }
      , [&](auto& e) { return std::make_tuple(1); }
      , [&](auto& e) { return std::make_tuple(3); }
        };
    EXPECT_EQ(3*sizeof(int), sizeof(actual));
    EXPECT_EQ(expected, actual.to_tuple());
}

TEST(FactoryTuple, HeterogeneousProduction)
{
    std::tuple<int, float, double> expected
      { 1, 3, 2 };
    FactoryTuple<int, float, double> actual
      { [&](auto& e) { return std::make_tuple(1); }
      , [&](auto& e) { return std::make_tuple(3); }
      , [&](auto& e) { return std::make_tuple(2); }
        };
    EXPECT_EQ(expected, actual.to_tuple());
}

TEST(FactoryTuple, HomogeneousAssignment)
{
    std::tuple<int, int, int> expected
      { 2, 1, 3 };
    FactoryTuple<int, int, int> actual
      { [&](auto& e) { return std::tie(std::get<0>(expected)); }
      , [&](auto& e) { return std::tie(std::get<1>(expected)); }
      , [&](auto& e) { return std::tie(std::get<2>(expected)); }
        };
    EXPECT_EQ(expected, actual.to_tuple());
}

TEST(FactoryTuple, HeterogeneousAssignment)
{
    std::tuple<int, float, double> expected
      { 1, 3, 2 };
    FactoryTuple<int, float, double> actual
      { [&](auto& e) { return std::tie(std::get<0>(expected)); }
      , [&](auto& e) { return std::tie(std::get<1>(expected)); }
      , [&](auto& e) { return std::tie(std::get<2>(expected)); }
        };
    EXPECT_EQ(expected, actual.to_tuple());
}

TEST(FactoryTuple, PaddedAssignment)
{
    std::tuple<int, char, float, char, char, double, int> expected
      { 1, 'a', 3, 'b', 'c', 2, 0 };
    FactoryTuple<int, char, float, char, char, double, int> actual
      { [&](auto& e) { return std::tie(std::get<0>(expected)); }
      , [&](auto& e) { return std::tie(std::get<1>(expected)); }
      , [&](auto& e) { return std::tie(std::get<2>(expected)); }
      , [&](auto& e) { return std::tie(std::get<3>(expected)); }
      , [&](auto& e) { return std::tie(std::get<4>(expected)); }
      , [&](auto& e) { return std::tie(std::get<5>(expected)); }
      , [&](auto& e) { return std::tie(std::get<6>(expected)); }
        };
    EXPECT_EQ(expected, actual.to_tuple());
}

TEST(FactoryTuple, DependantProductionSimple1)
{
    std::tuple<int, char, std::string> expected{3, 'a', "aaa"s};
    FactoryTuple<int, char, std::string> actual{
        [](auto& underConstructionTup) { return std::make_tuple(3); }
      , [](auto& underConstructionTup) { return std::make_tuple('a'); }
      , [](auto& underConstructionTup) {
            return std::make_tuple(
                underConstructionTup[0_c], underConstructionTup[1_c]
            );
        }
    };

    EXPECT_EQ(expected, actual.to_tuple());
}

TEST(FactoryTuple, DependantProductionSimple2)
{
    std::tuple<std::string, std::string> expected
      { "down the pipe", "out the pipe" };
    auto simple = [](auto& _) {
        return std::make_tuple("down the pipe");
    };
    auto dependant = [](auto& e) {
        std::string bottle{e[0_c]};
        bottle.replace(0, 4, "out");
        return std::make_tuple(std::move(bottle));
    };
    FactoryTuple<std::string, std::string> actual{simple, dependant};
    EXPECT_EQ(expected, actual.to_tuple());
}

