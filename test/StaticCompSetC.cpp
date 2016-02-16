#include "StaticSetC.h"
#include "MimicC.h"
#include "PositionC.h"
#include "SimpleMotionC.h"
#include "VelocityC.h"
#include <functional>
#include <tuple>
#include <aetee/aetee.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <SFML/Graphics.hpp>

using namespace aetee;



TEST(StaticSetC, ReactsToUpdates)
{
    int expected{3}, actual{0};
    // Test...
    StaticSetC<MimicC> sample{
        [&](auto&_) { return tupify([&](...){ actual += 1; }); }
        };
    sample.update(1), sample.update(1), sample.update(1);
    // Expectation...
    EXPECT_EQ(expected, actual);
}

TEST(StaticSetC, UpdatesInOrder)
{
    int expected{16}, actual{0};
    // Test...
    StaticSetC<MimicC, MimicC, MimicC> sample
      { [&](auto&_) { return tupify([&](...){ actual += 2; }); }
      , [&](auto&_) { return tupify([&](...){ actual += actual; }); }
      , [&](auto&_) { return tupify([&](...){ actual *= actual; }); }
        };
    sample.update(1);
    // Expectation...
    EXPECT_EQ(expected, actual);
}

// N.B. THIS IS THE MAIN MOTIVATION BEHIND THE USAGE OF FACTORYTUPLE
TEST(StaticSetC, ReferenceEarlierMembers)
{
    sf::Vector2f expected{0, 0}, initial{3, 4};
    // Test...
    StaticSetC<PositionC, VelocityC, SimpleMotionC> sample
      { [&](auto& e) { return tupify(initial); }
      , [&](auto& e) { return tupify(-initial);  }
      , [&](auto& e) { return std::tie(e[0_c], e[1_c]); }
        };
    sample.update(3), sample.update(-4), sample.update(2);
    // Expectation...
    EXPECT_EQ(expected, sample[0_c].value);
}
