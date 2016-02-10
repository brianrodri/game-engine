#ifndef HEADER_GAME_ENGINE_SRC_INC_COMPONENTS_TILE_H_INCLUDED
#define HEADER_GAME_ENGINE_SRC_INC_COMPONENTS_TILE_H_INCLUDED
#include "Component.h"
#include <SFML/Graphics.hpp>

//!
/**
 *
 */
typedef struct TileC : RenderComponent {
    //!
    /**
     *
     */
    TileC(const sf::Texture& txt, sf::FloatRect bounds) = default;

    //!
    /**
     *
     */
    ~TileC();

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

#endif
