#include "DynamicSetC.h"


BaseComponent& DynamicSetC::operator[](size_t i)
{
    return *members[i].get();
}

const BaseComponent& DynamicSetC::operator[](size_t i) const
{
    return *members[i].get();
}

void DynamicSetC::update(float dt)
{
    for (auto& ptr : members) {
        ProcessorComponent* dptr = dynamic_cast<ProcessorComponent*>(ptr.get());
        if (dptr != nullptr) { dptr->update(dt); }
    }
}

void DynamicSetC::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    for (auto& ptr : members) {
        PainterComponent* dptr = dynamic_cast<PainterComponent*>(ptr.get());
        if (dptr != nullptr) { dptr->draw(target, states); }
    }
}

std::unique_ptr<BaseComponent> DynamicSetC::detach()
{
    std::unique_ptr<BaseComponent> leaf{std::move(members.back())};
    members.pop_back();
    return leaf;
}
