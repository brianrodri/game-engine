#pragma once
#include "GameSettings.h"
#include <iostream>
#include <array>
#include <vector>

// Forward Declarations
class LevelAtlas;

class GameContext {

public:

    enum class DataType
      { EquipmentData, LevelMap, Conversation, Count };

    //! Begins with default debug-settings
    GameContext();

    //! Adds a file which can be relied on for a type of Data
    void addDataSource(DataType type, std::istream src);

    //! Generates settings from context accumulated in lifetime of this
    GameSettings generateSettings();

private:

    void loadEquipment(std::istream& src);
    void loadLevelMaps(std::istream& src);
    void loadConversations(std::istream& src);

} /*class GameContext*/;
