#include "FactoryTuple.h"
#include <string>
#include <tuple>
#include <utility>
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
    EXPECT_EQ(zeropair[0_c], 0);
    EXPECT_EQ(zeropair[1_c], 0);
    EXPECT_EQ(2*sizeof(int), sizeof(FactoryTuple<int, int>));
}

TEST(FactoryTuple, BasicFactoryConstructs)
{
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
