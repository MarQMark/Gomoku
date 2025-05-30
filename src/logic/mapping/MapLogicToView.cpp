#include "logic/mapping/MapLogicToView.h"

#include <algorithm>

// === Logic → Presentation ===
BoardViewDTO MapLogicToView::toBoardView(const Board& board, const GameState& state, const std::vector<GridPosition>& winningLine) {
    BoardViewDTO view;
    view.boardSize = Board::SIZE;
    view.currentTurn = state.currentPlayerTurn;
    view.gameStatus = state.status;
    view.moveNumber = state.moveNumber;

    // Convert current player color to name
    view.currentPlayerName = (state.currentPlayerTurn == BLACK) ? "Black Player" : "White Player";

    // Convert occupied positions to stone views
    for (const GridPosition& pos : board.getOccupiedPositions()) {
        StoneViewDTO stone{};
        stone.pos = pos;
        stone.previewColor = board.getColor(pos);
        view.stones.push_back(stone);
    }

    // Add winning line if game is won
    if (!winningLine.empty()) {
        view.winningLine = winningLine;
    }

    return view;
}

MoveViewDTO MapLogicToView::createMoveView(const bool success,
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



