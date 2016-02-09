#include <tuple>
#include <functional>
#include <aetee/aetee.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Entity.h"
#include "SimplePODComponents.h"

namespace ae = aetee;


TEST(Entity, ReactsToUpdates)
{
    int expected{3}, actual{0};
    auto factory = [&](auto&_) {
        return std::make_tuple([&](){ actual += 1; });
    };
    Entity<EchoComponent> dummy{factory};
    dummy.update(1), dummy.update(1), dummy.update(1);
    EXPECT_EQ(expected, actual);
}
