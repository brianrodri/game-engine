#pragma once
#include "Component.h"
#include <string>
#include <vector>
#include <experimental/string_view>
#include <SFML/Graphics.hpp>

struct PODC_Position : PODComponent { sf::Vector2f value; };
struct PODC_Velocity : PODC_Position { sf::Vector2f value; };
struct PODC_Acceleration : PODC_Position { sf::Vector2f value; };

template <typename R, typename... A>
struct PODC_Function { std::function<R(A...)> callback; };

struct PODC_Text : PODComponent { sf::String value; };

struct PODC_Sprite : PODComponent { sf::Sprite value; };

struct EchoComponent : ProcessorComponent {
    template <typename F>
    EchoComponent(F&& fn_) : fn{std::forward<F>(fn_)} {};
    virtual void update(float ft) override {
        fn();
    }
    std::function<void(void)> fn;
};
