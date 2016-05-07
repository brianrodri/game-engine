#pragma once
#include "GameContext.h"
#include <SFML/Graphics.hpp>
#include <functional>
#include <vector>
#include <array>
#include "Component.h"

class GameWorld {

    using Callback_t = std::function<void(const sf::Event&)>;

public:

    GameWorld(GameContext& context, GameSettings& settings);

    template <typename F>
    void subscribe(sf::Event::EventType e, F&& f)
    {
        m_callbacks[e].emplace_back(std::forward<F>(f));
    }

    void run();

    void processInput();

    void update(float dt);

    void render(sf::RenderStates = {});

private:

    sf::RenderWindow  m_window;
    std::array<std::vector<Callback_t>, sf::Event::EventType::Count>  m_callbacks;
    std::vector<Component::Ptr> m_components;

} /*class GameWorld*/;
