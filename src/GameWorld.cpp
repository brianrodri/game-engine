/*
 * =====================================================================================
 *
 *       Filename:  GameWorld.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  04/08/2016 06:16:26
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include "GameWorld.h"
#include "GameSettings.h"
#include <SFML/Graphics.hpp>


GameWorld::GameWorld(GameContext& context, GameSettings& settings)
  : m_window{
        sf::VideoMode{settings.getWindowDim().x, settings.getWindowDim().y}
      , context.getWindowTitle()
    }
{
    subscribe(sf::Event::EventType::Closed, [&](...) { m_window.close(); });
}


void GameWorld::processInput()
{
    sf::Event event{};
    while (m_window.pollEvent(event)) {
        for (auto& subj : m_callbacks[event.type]) { subj(event); }
    }
}


void GameWorld::update(float dt)
{
    for (auto& comp_ptr : m_components) {
        comp_ptr->update(dt);
    }
}


void GameWorld::render(sf::RenderStates stt)
{
    m_window.setActive();
    for (const auto& comp_ptr : m_components) {
        comp_ptr->draw(m_window, stt);
    }
    m_window.display();
}


void GameWorld::run()
{
    sf::Clock clock{};
    while (m_window.isOpen()) {
        processInput();
        update(clock.restart().asSeconds());
        render();
    }
}
