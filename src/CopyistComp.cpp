#include "CopyistComp.h"


void CopyistComp::draw(sf::RenderTarget& tar, sf::RenderStates stt) const
{
    functor(tar, stt);
}
