#include "DynamicSetC.h"


BaseComponent& DynamicSetC::operator[](size_t i)
{
    return *(members[i].get());
}

const BaseComponent& DynamicSetC::operator[](size_t i) const
{
    return *(members[i].get());
}

void DynamicSetC::update(float dt)
{
}

void DynamicSetC::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
}

std::unique_ptr<BaseComponent> DynamicSetC::detach()
{
    std::unique_ptr<BaseComponent> leaf{std::move(members.back())};
    members.pop_back();
    member_functionality.pop_back();
    return leaf;
}
