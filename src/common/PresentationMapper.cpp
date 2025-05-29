#include "common/PresentationMapper.h"
#include <algorithm>

// === Presentation → Logic ===
GridPosition PresentationMapper::commandToPosition(const PlaceStoneCommandDTO& cmd) {
    return GridPosition(cmd.x, cmd.y);
}

bool PresentationMapper::validateCommand(const PlaceStoneCommandDTO& cmd) {
    const GridPosition pos(cmd.x, cmd.y);
    return pos.isValid() && !cmd.playerId.empty();
}

// === Logic → Presentation ===
BoardViewDTO PresentationMapper::toBoardView(const Board& board, const GameState& state, const std::vector<GridPosition>& winningLine) {
    BoardViewDTO view;
    view.boardSize = Board::SIZE;
    view.currentTurn = state.currentPlayerTurn;
    view.gameStatus = state.status;
    view.moveNumber = state.moveNumber;

    // Convert current player color to name
    view.currentPlayerName = (state.currentPlayerTurn == StoneColor::BLACK)
                            ? "Black Player" : "White Player";

    // Convert occupied positions to stone views
    for (const GridPosition& pos : board.getOccupiedPositions()) {
        StoneViewDTO stone;
        stone.x = pos.x;
        stone.y = pos.y;
        stone.color = board.getColor(pos);
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

MoveResultDTO PresentationMapper::createMoveResult(bool success,
                                        const Board& board,
                                        const GameState& state,
                                        const std::string& error,
                                        const std::vector<GridPosition>& winningLine) {
    MoveResultDTO result;
    result.success = success;

    if (success) {
        result.boardState = toBoardView(board, state, winningLine);

        if (state.status == GameStatus::BLACK_WINS) {
            result.winner = StoneColor::BLACK;
        } else if (state.status == GameStatus::WHITE_WINS) {
            result.winner = StoneColor::WHITE;
        }
    } else {
        result.errorMessage = error;
        result.boardState = toBoardView(board, state);
    }

    return result;
}