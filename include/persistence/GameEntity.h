
#ifndef GAMEDATA_H
#define GAMEDATA_H
#include <string>
#include <vector>

#include "MoveEntity.h"
#include "PlayerEntity.h"
#include "logic/Board.h"

struct GameEntity {
    std::string gameId;
    PlayerEntity player1;
    PlayerEntity player2;
    std::vector<MoveEntity> moveHistory;
};

#endif
