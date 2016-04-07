#include "VelocityComp.h"


VelocityComp::VelocityComp(float x, float y) : value{x, y}
{
}

VelocityComp::VelocityComp(const sf::Vector2f& src) : value{src}
{
}
