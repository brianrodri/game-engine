#pragma once
#include "Component.h"
#include <string>
#include <vector>
#include <experimental/string_view>
#include <SFML/Graphics.hpp>

struct PODC_Position : PODComponent { sf::Vect2f value; };
struct PODC_Velocity : PODC_Position { sf::Vect2f value; };
struct PODC_Acceleration : PODC_Position { sf::Vect2f value; };

struct PODC_Text : PODComponent { sf::String value; };

struct PODC_Sprite : PODComponent { sf::Sprite value; };
