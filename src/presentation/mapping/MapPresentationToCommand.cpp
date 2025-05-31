#include "presentation/mapping/MapPresentationToCommand.h"

GridPosition MapPresentationToCommand::relativeToGrid(const glm::vec2 relativePos, const int boardSize) {
    int gridX = static_cast<int>(relativePos.x * (boardSize - 1) + 0.5f);
    int gridY = static_cast<int>(relativePos.y * (boardSize - 1) + 0.5f);
    return {gridX, gridY};
}

MouseCommandDTO MapPresentationToCommand::toMouseCommandDTO(const glm::vec2 relativePosition, const int boardSize) {
    return MouseCommandDTO(relativeToGrid(relativePosition, boardSize));
}

