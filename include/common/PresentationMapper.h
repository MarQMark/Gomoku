#ifndef PRESENTATIONMAPPER_H
#define PRESENTATIONMAPPER_H

#include "logic/Board.h"
#include "logic/DTO/CommandDTOs.h"
#include "presentation/DTO/ViewModelDTOs.h"
#include <vector>

class PresentationMapper {
public:
    // === Presentation → Logic ===
    static GridPosition commandToPosition(const PlaceStoneCommandDTO& cmd);
    static bool validateCommand(const PlaceStoneCommandDTO& cmd);

    // === Logic → Presentation ===
    static BoardViewDTO toBoardView(const Board& board, const GameState& state, const std::vector<GridPosition>& winningLine = {});
    static MoveResultDTO createMoveResult(bool success, const Board& board, const GameState& state, const std::string& error = "", const std::vector<GridPosition>& winningLine = {});
};

#endif // PRESENTATIONMAPPER_H