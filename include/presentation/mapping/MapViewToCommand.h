#ifndef PRESENTATIONTOCOMMAND_H
#define PRESENTATIONTOCOMMAND_H
#include "glm/vec2.hpp"
#include "logic/Board.h"
#include "logic/DTO/CommandDTOs.h"

class MapViewToCommand {
public:
    static MouseCommandDTO toMouseCommandDTO(glm::vec2 relativePosition, int boardSize);
    static GameSetupCommandDTO toGameSetupCommandDTO(GameMode gameMode, AIDifficulty difficulty);

    static std::pair<std::string, std::string> mapGameModeToPlayerName(GameMode mode, AIDifficulty difficulty);

    static GridPosition relativeToGrid(glm::vec2 relativePos, int boardSize);

};

#endif
