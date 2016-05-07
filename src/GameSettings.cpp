#include "GameSettings.h"


GameSettings::GameSettings()
{
}

GameSettings::GameSettings(const GameSettings& src)
{
}

GameSettings::GameSettings(GameSettings&& src)
{
}

GameSettings::~GameSettings()
{
}

GameSettings& GameSettings::operator=(const GameSettings& src)
{
    return *this;
}

GameSettings& GameSettings::operator=(GameSettings&& src)
{
    return *this;
}

sf::Vector2u GameSettings::getWindowDim() const
{
    return {800, 600};
}
