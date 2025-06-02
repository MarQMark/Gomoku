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
        std::vector<GridViewPosition> winningViewLines;
        winningViewLines.reserve(winningLine.size());
        for (const GridPosition& line : winningLine) {
            winningViewLines.emplace_back(line.x, line.y);
        }
        view.winningLine = winningViewLines;
    }

    return view;
}

StatsViewDTO MapLogicToView::createStatsViewDTO(const IPlayer* player1, const IPlayer* player2, const double elapsedTime, const BoardState &boardState) {
    StatsViewDTO view;
    if (player1->getColor() == BLACK) {
        view.blackPlayer = player1->getName();
        view.whitePlayer = player2->getName();
    } else {
        view.whitePlayer = player1->getName();
        view.blackPlayer = player2->getName();
    }
    view.turn = boardState.moveNumber;
    view.currentTime = elapsedTime;
    view.lastPosition = GridViewPosition(boardState.latestMove.x, boardState.latestMove.y);
    view.gameStatus = boardState.status;
    view.currentPlayer = boardState.currentPlayer->getName();
    return view;
}

StoneViewDTO MapLogicToView::createStoneViewDTO(const bool isValid, const GridPosition gridPosition, const StoneColor stoneColor) {
    StoneViewDTO view;
    view.isValidPosition = isValid;
    view.pos = GridViewPosition(gridPosition.x, gridPosition.y);
    view.previewColor = mapToViewColor(stoneColor);
    return view;
}

GameCompleteViewDTO MapLogicToView::createGameCompletedView(const StoneColor winnerColor, const GameStatus gameStatus,
                                                             const std::vector<GridPosition> &winningLine) {
    GameCompleteViewDTO viewDTO;
    viewDTO.winningLine = mapToViewPositions(winningLine);
    viewDTO.winnerColor = mapToViewColor(winnerColor);
    viewDTO.status = gameStatus;

    return viewDTO;
}

std::vector<GridViewPosition> MapLogicToView::mapToViewPositions(const std::vector<GridPosition>& gridPositions) {
    std::vector<GridViewPosition> viewPositions;
    viewPositions.reserve(gridPositions.size());

    for (const GridPosition& pos : gridPositions) {
        viewPositions.emplace_back(pos.x, pos.y);
    }
    return viewPositions;
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
                                        const IPlayer& player,
                                        const std::string& error) {
    MoveViewDTO result;
    result.success = success;
    result.stone = stone;
    result.boardView = view;
    result.currentPlayerName = player.getName();

    if (!success) {
        result.errorMessage = error;
    }

    return result;
}



