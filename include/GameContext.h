#pragma once
#include "GameSettings.h"
#include <iostream>
#include <array>
#include <vector>

// Forward Declarations
class LevelAtlas;

//! A mediator of Game-specific details
/**
 *  Meant to hold onto global data that other Game instances may have an
 *  interest in, for example equipment data, levels, npc data, shop data,
 *  and so on.  
 */
class GameContext {

public:

    enum class DataType {
        EquipmentData, LevelMap, Conversation, Count
    };

    //! Begins with default debug-settings
    GameContext();

    //! Adds a stream from which data may be extracted
    void loadEquipment(std::istream& src);
    void loadLevelMaps(std::istream& src);
    void loadConversations(std::istream& src);

    //! Generates settings from context accumulated in lifetime of this
    GameSettings generateSettings();

    //! Returns window title
    const char * getWindowTitle() const;

private:

} /*class GameContext*/;
