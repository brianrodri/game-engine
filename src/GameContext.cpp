#include "GameContext.h"

GameContext::GameContext()
{
}

GameSettings GameContext::generateSettings()
{
    return {};
}

void GameContext::addDataSource(DataType type, std::istream src)
{
    switch (type) {
        case DataType::EquipmentData:
            loadEquipment(src);
            break;

        case DataType::LevelMap:
            loadLevelMaps(src);
            break;

        case DataType::Conversation:
            loadConversations(src);
            break;
    }
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

