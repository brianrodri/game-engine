#pragma once
using uint = unsigned int;

struct BattleStats {

    BattleStats();
    BattleStats(const BattleStats& src) = default;
    BattleStats& operator=(const BattleStats& src) = default;

    uint level = 0;
    uint xp = 0;
    uint xpNext = 5;

    uint hp = 10;
    uint hpMax = 10;
    uint mp = 3;
    uint mpMax = 3;

    uint att = 3;
    uint def = 3;
    uint mag = 3;
    uint magdef = 3;

    uint spd = 2;

} /*struct BattleStats*/;
