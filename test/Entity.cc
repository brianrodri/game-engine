#include <tuple>
#include <functional>
#include <aetee/aetee.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <SFML/Graphics.hpp>
#include "Entity.h"
#include "PODComponents.h"
#include "Echo.h"

using namespace aetee;


TEST(Entity, ReactsToUpdates)
{
    int expected{3};
    int actual{0};
    auto incrementer = [&](...){ actual += 1; };
    auto factory = [&](auto&_) { return std::make_tuple(incrementer); };
    Entity<EchoC> dummy{factory};
    dummy.update(1), dummy.update(1), dummy.update(1);
    EXPECT_EQ(expected, actual);
}

TEST(Entity, UpdateInOrder)
{
    int expected{16};
    int actual{0};
    auto addOp = [&](...){ actual += 2; };      // a = 2
    auto mulOp = [&](...){ actual += actual; }; // a = 4
    auto expOp = [&](...){ actual *= actual; }; // a = 16
    Entity<EchoC, EchoC, EchoC> dummy
      { [&](auto&_) { return std::make_tuple(addOp); }
      , [&](auto&_) { return std::make_tuple(mulOp); }
      , [&](auto&_) { return std::make_tuple(expOp); }
        };
    dummy.update(1);
    EXPECT_EQ(expected, actual);
}

//! N.B.:  MAIN MOTIVATION BEHIND USAGE OF FACTORYTUPLE
TEST(Entity, ReferenceEarlierMembers)
{
    sf::Vector2f expected{0, 0};
    sf::Vector2f initial{3, 4};

    Entity<PositionC, VelocityC, EchoC> dummy
      { [&](auto& e) { return std::make_tuple(initial); }
      , [&](auto& e) { return std::make_tuple(-e[0_c].value);  }
      , [&](auto& e) {
            auto onUpdate = [&](float i) mutable {
                auto& pos = std::get<PositionC>(e);
                auto& vel = std::get<VelocityC>(e);
                pos.value += (i * vel.value);
            };
            return std::make_tuple(onUpdate);
        }};

    sf::Vector2f& actual = dummy[0_c].value;
    EXPECT_EQ(initial, actual);
    dummy.update(3), dummy.update(-4), dummy.update(2);
    EXPECT_EQ(expected, actual);
}
