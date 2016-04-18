#pragma once


class GameSettings {

public:

    GameSettings();
    GameSettings(const GameSettings&);
    GameSettings(GameSettings&&);

    ~GameSettings();

    GameSettings& operator=(const GameSettings&);
    GameSettings& operator=(GameSettings&&);

private:

} /*class GameSettings*/;
