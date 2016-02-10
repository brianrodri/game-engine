#include <tuple>
#include <functional>
#include <aetee/aetee.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <SFML/Graphics.hpp>
#include "Entity.h"
#include "PODComponents.h"
#include "Mimic.h"

using namespace aetee;


TEST(Entity, ReactsToUpdates)
{
    int expected{3}, actual{0};

    Entity<MimicC> sample{
        [&](auto&_) { return std::make_tuple([&](...){ actual += 1; }); }
        };
    sample.update(1), sample.update(1), sample.update(1);

    EXPECT_EQ(expected, actual);
}

TEST(Entity, UpdateInOrder)
{
    int expected{16};
    int actual{0};
    auto addOp = [&](...){ actual += 2; };
    auto dblOp = [&](...){ actual += actual; };
    auto expOp = [&](...){ actual *= actual; };

    Entity<MimicC, MimicC, MimicC> sample
      { [&](auto&_) { return std::make_tuple(addOp); }
      , [&](auto&_) { return std::make_tuple(dblOp); }
      , [&](auto&_) { return std::make_tuple(expOp); }
        };
    sample.update(1);

    EXPECT_EQ(expected, actual);
}

//! N.B.:  MAIN MOTIVATION BEHIND USAGE OF FACTORYTUPLE
TEST(Entity, ReferenceEarlierMembers)
{
    sf::Vector2f expected{0, 0};
    sf::Vector2f initial{3, 4};

    Entity<PositionC, VelocityC, MimicC> sample
      { [&](auto& e) { return std::make_tuple(initial); }
      , [&](auto& e) { return std::make_tuple(-e[0_c].value);  }
      , [&](auto& e) {
            auto& p = std::get<PositionC>(e);
            auto& v = std::get<VelocityC>(e);
            return std::make_tuple([&](float i) { p.value += (v.value * i); });
        }};
    sf::Vector2f& actual = sample[0_c].value;

    EXPECT_EQ(initial, actual);
    sample.update(3), sample.update(-4), sample.update(2);
    EXPECT_EQ(expected, actual);
}
