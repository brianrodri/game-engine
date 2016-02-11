#include "VelocityC.h"


VelocityC::VelocityC(float x, float y) : value{x, y}
{
}

VelocityC::VelocityC(const sf::Vector2f& src) : value{src}
{
}
