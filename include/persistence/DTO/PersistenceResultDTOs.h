#ifndef PERSISTENCERESULTDTOS_H
#define PERSISTENCERESULTDTOS_H

#include <string>
#include <vector>
#include "persistence/Models.h"

struct LoadGameResultDTO {
    std::string gameId;
    PlayerModel player1;
    PlayerModel player2;
    std::vector<MoveModel> moveHistory;
    GameStateModel gameState;

    LoadGameResultDTO() : gameId("") {}
};

#endif