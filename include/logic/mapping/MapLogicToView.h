#ifndef PRESENTATIONMAPPER_H
#define PRESENTATIONMAPPER_H

#include "logic/Board.h"
#include "presentation/DTO/ViewModelDTOs.h"
#include <vector>

// Maps from the Logic to the Presentation Layer
class MapLogicToView {
public:
    static BoardViewDTO toBoardView(const Board& board, const GameState& state, const std::vector<GridPosition>& winningLine);
    static MoveViewDTO createMoveView(bool success, const BoardViewDTO &view, const StoneViewDTO &stone, const std::string &error = "");
};

#endif