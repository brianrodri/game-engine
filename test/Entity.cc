#include <tuple>
#include <functional>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Entity.h"
#include "SimplePODComponents.h"



/*
TEST(Entity, ReactsToUpdates)
{
    int expected{3}, actual{0};
    auto addOne = [&]() { actual += 1; };
    Entity<EchoComponent> dummy{ [&](auto& _){ return std::make_tuple(addOne); } };
    dummy.update(1), dummy.update(1), dummy.update(1);
    EXPECT_EQ(expected, actual);
}
*/
