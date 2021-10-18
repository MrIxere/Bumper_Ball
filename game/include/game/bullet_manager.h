#pragma once
#include <SFML/System/Time.hpp>

#include "game_globals.h"

namespace game
{
    struct Bullet
    {
        float remainingTime = 0.0f;
        PlayerNumber playerNumber = INVALID_PLAYER;
    };

    class GameManager;
}
