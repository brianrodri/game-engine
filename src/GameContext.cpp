#include "GameContext.h"

GameContext::GameContext()
{
}

GameSettings GameContext::generateSettings()
{
    return {};
}

void GameContext::loadEquipment(std::istream& src)
{
}

void GameContext::loadLevelMaps(std::istream& src)
{
}

void GameContext::loadConversations(std::istream& src)
{
}

const char * GameContext::getWindowTitle() const
{
    return "mint-engine";
}
