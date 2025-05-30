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
        stone.isLastMove = (pos == state.lastMove);
        stone.isWinningStone = std::find(winningLine.begin(),
                                       winningLine.end(),
                                       pos) != winningLine.end();

        view.stones.push_back(stone);
    }

    // Add winning line if game is won
    if (!winningLine.empty()) {
        view.winningLine = winningLine;
    }

    return view;
}

MoveViewDTO MapLogicToView::createMoveView(const bool success,
                                        const Board& board,
                                        const GameState& state,
                                        const std::string& error,
                                        const std::vector<GridPosition>& winningLine) {
    MoveViewDTO result;
    result.success = success;

    if (success) {
        result.boardState = toBoardView(board, state, winningLine);

        if (state.status == BLACK_WINS) {
            result.winner = BLACK;
        } else if (state.status == WHITE_WINS) {
            result.winner = WHITE;
        }
    } else {
        result.errorMessage = error;
        result.boardState = toBoardView(board, state);
    }

    return result;
}



