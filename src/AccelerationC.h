#include "AccelerationC.h"


AccelerationC::AccelerationC(float x, float y) : value{x, y}
{
}

AccelerationC::AccelerationC(const sf::Vector2f& src) : value{src}
{
}
