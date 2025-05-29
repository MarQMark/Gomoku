#include "common/PresentationMapper.h"
#include <algorithm>

#include "presentation/input/InputDefs.h"

// === Presentation → Logic ===
GridPosition PresentationMapper::commandToPosition(const PlaceStoneCommandDTO& cmd) {
    return GridPosition(cmd.x, cmd.y);
}

bool PresentationMapper::validatePlaceStoneCommand(const PlaceStoneCommandDTO& cmd) {
    const GridPosition pos(cmd.x, cmd.y);
    return pos.isValid() && !cmd.playerId.empty();
}

bool PresentationMapper::validateMouseCommand(const MouseCommandDTO& cmd) {
    return cmd.relativeBoardX >= 0.0f && cmd.relativeBoardX <= 1.0f &&
           cmd.relativeBoardY >= 0.0f && cmd.relativeBoardY <= 1.0f &&
           !cmd.playerId.empty();
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

// === Coordinate conversion helpers ===
glm::vec2 PresentationMapper::boardAreaToGridRelative(glm::vec2 boardPos, glm::vec2 boardSize,
                                                     glm::vec2 mousePos, glm::vec2 viewportSize) {
    // First normalize mouse coordinates to 0-1 viewport space
    const glm::vec2 normalizedMouse = mousePos / viewportSize;

    // Check if mouse is within board area
    if (normalizedMouse.x < boardPos.x || normalizedMouse.x > boardPos.x + boardSize.x ||
        normalizedMouse.y < boardPos.y || normalizedMouse.y > boardPos.y + boardSize.y) {
        return glm::vec2(-1.0f, -1.0f); // Invalid position
    }

    // Convert to relative coordinates within the grid area (0.0 to 1.0)
    const glm::vec2 relativeMouse = (normalizedMouse - boardPos) / boardSize;

    return relativeMouse;
}

glm::vec2 PresentationMapper::gridToViewPosition(int gridX, int gridY, glm::vec2 boardPos, glm::vec2 boardSize, int boardSizeGrid) {
    // Calculate grid spacing (distance between intersections)
    float gridSpacing = 1.0f / (boardSizeGrid - 1);

    // Calculate relative position within the grid area
    glm::vec2 relativeIntersection = glm::vec2(
        static_cast<float>(gridX) * gridSpacing,
        static_cast<float>(gridY) * gridSpacing
    );

    // Convert to actual position within the board area
    glm::vec2 actualIntersection = boardPos + relativeIntersection * boardSize;

    return actualIntersection;
}

float PresentationMapper::calculateStoneSize(glm::vec2 boardSize, int boardSizeGrid) {
    float gridSpacing = 1.0f / (boardSizeGrid - 1);
    return gridSpacing * boardSize.x; // Use board width as reference
}