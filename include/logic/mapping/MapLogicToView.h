#ifndef PRESENTATIONMAPPER_H
#define PRESENTATIONMAPPER_H

#include "logic/Board.h"
#include "presentation/DTO/ViewModelDTOs.h"
#include <vector>

class MapLogicToView {
public:
    static BoardViewDTO mapToBoardViewDTO(const Board& board, const BoardState& state, const std::vector<GridPosition>& winningLine);

    static StoneViewDTO createStoneViewDTO(bool isValid, GridPosition gridPosition, StoneColor stoneColor);

    static ViewColor mapToViewColor(StoneColor stoneColor);

    static MoveViewDTO createMoveViewDTO(bool success, const BoardViewDTO &view, const StoneViewDTO &stone, const std::string &error = "");
};

#endif