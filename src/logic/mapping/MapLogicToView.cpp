#include "logic/mapping/MapLogicToView.h"

#include "logic/player/IPlayer.h"

BoardViewDTO MapLogicToView::mapToBoardViewDTO(const Board& board, const BoardState& state, const std::vector<GridPosition>& winningLine) {
    BoardViewDTO view;
    view.boardSize = Board::SIZE;
    view.currentTurn = mapToViewColor(state.currentPlayer->getColor());
    view.currentPlayerName = (state.currentPlayer->getColor() == BLACK) ? "Black Player" : "White Player";
    view.gameStatus = state.status;
    view.moveNumber = state.moveNumber;


    // Convert occupied positions to stone views
    for (const Move& move : board.getOccupiedPositions()) {
        view.stones.push_back(createStoneViewDTO(move.position.isValid(), move.position, move.color));
    }

    // Add winning line if game is won
    if (!winningLine.empty()) {
        std::vector<ViewPosition> winningViewLines;
        winningViewLines.reserve(winningLine.size());
        for (const GridPosition& line : winningLine) {
            winningViewLines.emplace_back(line.x, line.y);
        }
        view.winningLine = winningViewLines;
    }

    return view;
}

StoneViewDTO MapLogicToView::createStoneViewDTO(const bool isValid, const GridPosition gridPosition, const StoneColor stoneColor) {
    StoneViewDTO view;
    view.isValidPosition = isValid;
    view.pos = ViewPosition(gridPosition.x, gridPosition.y);
    view.previewColor = mapToViewColor(stoneColor);
    return view;
}

ViewColor MapLogicToView::mapToViewColor(const StoneColor stoneColor) {
    switch (stoneColor) {
        case BLACK: return ViewColor::BLACK;
        case WHITE: return ViewColor::WHITE;
        default: return ViewColor::NONE;
    }
}

MoveViewDTO MapLogicToView::createMoveViewDTO(const bool success,
                                        const BoardViewDTO& view,
                                        const StoneViewDTO& stone,
                                        const std::string& error) {
    MoveViewDTO result;
    result.success = success;
    result.stone = stone;
    result.boardView = view;

    if (!success) {
        result.errorMessage = error;
    }

    return result;
}



