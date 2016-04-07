#include "CopyistComp.h"


CopyistComp::draw(sf::RenderTarget& tar, sf::RenderStates stt) const
{
    functor(tar, stt);
}
