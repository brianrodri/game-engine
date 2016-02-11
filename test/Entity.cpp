#include <tuple>
#include <functional>
#include <aetee/aetee.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <SFML/Graphics.hpp>
#include "Entity.h"
#include "MimicC.h"
#include "SimpleMotionC.h"
#include "PositionC.h"
#include "VelocityC.h"

using namespace aetee;


TEST(Entity, ReactsToUpdates)
{
    int expected{3}, actual{0};
    // Test...
    Entity<MimicC> sample{
        [&](auto&_) { return tupify([&](...){ actual += 1; }); }
        };
    sample.update(1), sample.update(1), sample.update(1);
    // Expectation...
    EXPECT_EQ(expected, actual);
}

TEST(Entity, UpdateInOrder)
{
    int expected{16}, actual{0};
    // Test...
    Entity<MimicC, MimicC, MimicC> sample
      { [&](auto&_) { return tupify([&](...){ actual += 2; }); }
      , [&](auto&_) { return tupify([&](...){ actual += actual; }); }
      , [&](auto&_) { return tupify([&](...){ actual *= actual; }); }
        };
    sample.update(1);
    // Expectation...
    EXPECT_EQ(expected, actual);
}

// N.B.  THIS IS THE MAIN MOTIVATION BEHIND THE USAGE OF FACTORYTUPLE
TEST(Entity, ReferenceEarlierMembers)
{
    sf::Vector2f expected{0, 0}, initial{3, 4};
    // Test...
    Entity<PositionC, VelocityC, SimpleMotionC> sample
      { [&](auto& e) { return tupify(initial); }
      , [&](auto& e) { return tupify(-initial);  }
      , [&](auto& e) { return std::tie(e[0_c], e[1_c]); }
        };
    sample.update(3), sample.update(-4), sample.update(2);
    // Expectation...
    EXPECT_EQ(expected, sample[0_c].value);
}
