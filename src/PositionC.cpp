#include "PositionC.h"


PositionC::PositionC(float x, float y) : value{x, y}
{
}

PositionC::PositionC(const sf::Vector2f& src) : value{src}
{
}
