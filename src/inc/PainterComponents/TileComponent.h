#pragma once
#include "Component.h"
#include <SFML/Graphics.hpp>

//!
/**
 *
 */
typedef struct TileComponent : RenderComponent {
    //!
    /**
     *
     */
    RENC_Tile(const sf::Texture& txt, sf::FloatRect bounds) = default;

    //!
    /**
     *
     */
    ~RENC_Tile();

    //!
    /**
     *
     */
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        target.draw(m_sprite, states);
    }

private:
    //std::reference_wrapper<const sf::Texture> text;
};
