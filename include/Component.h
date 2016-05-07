#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

class Component : public sf::Drawable, public sf::Transformable {
public:

    using Ptr = std::unique_ptr<Component>;

    virtual ~Component() = default;
    virtual void update(float ft);
    virtual void draw(const sf::RenderTarget&, sf::RenderStates) const;

} /*struct Component*/;
