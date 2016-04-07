#include "PositionComp.h"


PositionComp::PositionComp(float x, float y) : value{x, y}
{
}

PositionComp::PositionComp(const sf::Vector2f& src) : value{src}
{
}
