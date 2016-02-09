#include <tuple>
#include <functional>
#include <aetee/aetee.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Entity.h"
#include "SimplePODComponents.h"


TEST(Entity, ReactsToUpdates)
{
    int expected{3};
    int actual{0};
    auto incrementer = [&](){ actual += 1; };
    auto factory = [&](auto&_) { return std::make_tuple(incrementer); };
    Entity<EchoComponent> dummy{factory};
    dummy.update(1), dummy.update(1), dummy.update(1);
    EXPECT_EQ(expected, actual);
}
