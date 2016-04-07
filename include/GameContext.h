#pragma once
#include <iostream>

// Forward Declarations
class LevelAtlas;

class GameContext {

public:

    public enum class DataType
      { EquipmentData, LevelMap, Conversation, Count };

    //! Begins with default debug-settings
    GameContext();

    //! Adds a file which can be relied on for a type of Data
    void addDataSource(DataType type, std::istream&& src);

    //! Generates data from the
    void generate();

private:

    std::array<std::vector<std::istream>, DataType::Count>  m_dataSources;

    void loadEquipment(std::istream& src);
    void loadLevelMaps(std::istream& src);
    void loadConversations(std::istream& src);

} /*class GameContext*/;
