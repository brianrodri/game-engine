#include "GameContext.h"

GameContext::GameContext()
{
}

void GameContext::addDataSource(DataType type, std::istream src)
{
    //m_dataSources[static_cast<int>(type)].emplace_back(src);
}

void GameContext::generate()
{
    for (std::istream& src : m_dataSources[0]) { loadEquipment(src); }
    for (std::istream& src : m_dataSources[1]) { loadLevelMaps(src); }
    for (std::istream& src : m_dataSources[2]) { loadConversations(src); }
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

