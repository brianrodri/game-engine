#include "DynamicCompSetC.h"


BaseComponent& DynamicCompSetC::operator[](size_t i)
{
    return *members[i];
}

const BaseComponent& DynamicCompSetC::operator[](size_t i) const
{
    return *members[i];
}

void DynamicCompSetC::update(float dt)
{
    constexpr auto canUpdateFn = BaseComponent::UpdateChecker{};
    for (auto& ptr : members) {
        if (canUpdateFn(ptr)) { ptr->update(dt); }
    }
}

void DynamicCompSetC::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    constexpr auto canRenderFn = BaseComponent::RenderChecker{};
    for (auto& ptr : members) {
        if (canRenderFn(ptr)) { ptr->draw(target, states); }
    }
}
