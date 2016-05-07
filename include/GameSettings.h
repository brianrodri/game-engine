#pragma once
#include <SFML/Graphics.hpp>


class GameSettings {

public:

    GameSettings();
    GameSettings(const GameSettings&);
    GameSettings(GameSettings&&);

    ~GameSettings();

    GameSettings& operator=(const GameSettings&);
    GameSettings& operator=(GameSettings&&);


    sf::Vector2u getWindowDim() const;

private:

} /*class GameSettings*/;
