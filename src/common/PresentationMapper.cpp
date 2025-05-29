#include "common/PresentationMapper.h"
#include <algorithm>

// === Presentation → Logic ===
GridPosition PresentationMapper::commandToPosition(const MouseCommandDTO& cmd) {
    return GridPosition(cmd.gridPosition.x, cmd.gridPosition.y);
}

bool PresentationMapper::validatePlaceStoneCommand(const MouseCommandDTO& cmd) {
    return cmd.gridPosition.isValid() && !cmd.playerId.empty();
}

MouseCommandDTO PresentationMapper::toMouseCommandDTO(glm::vec2 relativePosition, const std::string &playerId) {
    return MouseCommandDTO(relativeToGrid(relativePosition), playerId);
}

GridPosition PresentationMapper::relativeToGrid(const glm::vec2 relativePos) {
    int gridX = static_cast<int>(relativePos.x * (Board::SIZE - 1) + 0.5f);
    int gridY = static_cast<int>(relativePos.y * (Board::SIZE - 1) + 0.5f);

    // Clamp to valid range
    gridX = std::max(0, std::min(Board::SIZE - 1, gridX));
    gridY = std::max(0, std::min(Board::SIZE - 1, gridY));

    return {gridX, gridY};
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

MoveResultDTO PresentationMapper::createMoveResult(const bool success,
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

// === Coordinate conversion helpers ===
glm::vec2 PresentationMapper::boardAreaToGridRelative(const glm::vec2 boardPos, const glm::vec2 boardSize,
                                                     const glm::vec2 mousePos, const glm::vec2 viewportSize) {
    const glm::vec2 normalizedMouse = mousePos / viewportSize;

    if (normalizedMouse.x < boardPos.x || normalizedMouse.x > boardPos.x + boardSize.x ||
        normalizedMouse.y < boardPos.y || normalizedMouse.y > boardPos.y + boardSize.y) {
        return glm::vec2(-1.0f, -1.0f);
    }

    // Convert to relative coordinates within the grid area (0.0 to 1.0)
    const glm::vec2 relativeMouse = (normalizedMouse - boardPos) / boardSize;

    return relativeMouse;
}

glm::vec2 PresentationMapper::gridToViewPosition(const int gridX, const int gridY, const glm::vec2 boardPos, const glm::vec2 boardSize, const int boardSizeGrid) {
    // Calculate grid spacing (distance between intersections)
    const float gridSpacing = 1.0f / (boardSizeGrid - 1);

    // Calculate relative position within the grid area
    const auto relativeIntersection = glm::vec2(
        static_cast<float>(gridX) * gridSpacing,
        static_cast<float>(gridY) * gridSpacing
    );

    // Convert to actual position within the board area
    const glm::vec2 actualIntersection = boardPos + relativeIntersection * boardSize;

    return actualIntersection;
}

float PresentationMapper::calculateStoneSize(const glm::vec2 boardSize, const int boardSizeGrid) {
    const float gridSpacing = 1.0f / (boardSizeGrid - 1);
    return gridSpacing * boardSize.x; // Use board width as reference
}


