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
    EXPECT_TRUE(true);
}

TEST(FactoryTuple, DefaultConstructs)
{
    FactoryTuple<int, int> zeropair{};
    EXPECT_EQ(2*sizeof(int), sizeof(FactoryTuple<int, int>));
    EXPECT_EQ(zeropair[0_c], 0);
    EXPECT_EQ(zeropair[1_c], 0);
}

TEST(FactoryTuple, ProperlyAllocated)
{
    std::array<int, 3> expected{2, 1, 3};
    FactoryTuple<int, int, int> actual{
        [](auto& _) -> std::tuple<int> { return {2}; }
      , [](auto& e) -> std::tuple<int> { return {e[0_c] - 1}; }
      , [](auto& e) -> std::tuple<int> { return {e[1_c] + 2}; }
        };
    EXPECT_EQ(sizeof(actual), sizeof(expected));
    EXPECT_EQ(actual[0_c], expected[0]);
    EXPECT_EQ(actual[1_c], expected[1]);
    EXPECT_EQ(actual[2_c], expected[2]);
}

/*
TEST(FactoryTuple, BasicFactoryConstructs)
{
    using std::literals::string_literals::operator""s;
    auto makefoo = [](auto&) { return std::forward_as_tuple("foo"s); };
    auto makebar = [](auto&) { return std::forward_as_tuple("bar"s); };

    FactoryTuple<std::string, std::string> strpair{makefoo, makebar};

    EXPECT_EQ(strpair[0_c], "foo");
    EXPECT_EQ(strpair[1_c], "bar");
}

TEST(FactoryTuple, ComplexFactoryConstructs)
{
    auto simple = [](auto&) { return std::forward_as_tuple("down the pipe!"s); };
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
