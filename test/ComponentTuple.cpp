#include "ComponentTuple.h"
#include "MimicComp.h"
#include "PhysicsComps.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <SFML/Graphics.hpp>
#include <functional>
#include <tuple>



TEST(ComponentTuple, ReactsToUpdates)
{
    int expected{3}, actual{0};
    ComponentTuple<MimicComp> sample
      { [&](auto&_) { return std::make_tuple([&](...) { actual += 1; }); }
        };
    sample.update(1), sample.update(1), sample.update(1);
}

TEST(ComponentTuple, UpdatesInOrder)
{
    int expected{16}, actual{0};

    ComponentTuple<MimicComp, MimicComp, MimicComp> sample
      { [&](auto&_) { return std::make_tuple([&](...){ actual += 2; }); }
      , [&](auto&_) { return std::make_tuple([&](...){ actual += actual; }); }
      , [&](auto&_) { return std::make_tuple([&](...){ actual *= actual; }); }
        };
    sample.update(1);

    EXPECT_EQ(expected, actual);
}

// N.B. THIS IS THE MAIN MOTIVATION BEHIND THE USAGE OF FACTORYTUPLE
TEST(ComponentTuple, ReferenceEarlierMembers)
{
    sf::Vector2f expected{0, 0}, initial{3, 4};

    ComponentTuple<PositionComp, VelocityComp, MotionComp> sample
      { [&](auto& e) { return std::make_tuple(initial); }
      , [&](auto& e) { return std::make_tuple(-initial);  }
      , [&](auto& e) { return std::make_tuple(&e[0_c], &e[1_c]); }
        };
    sample.update(3), sample.update(-4), sample.update(2);

    EXPECT_EQ(expected, sample[0_c].value);
}

