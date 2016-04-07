#include "AccelComp.h"


AccelComp::AccelComp(float x, float y) : value{x, y}
{
}

AccelComp::AccelComp(const sf::Vector2f& src) : value{src}
{
}
