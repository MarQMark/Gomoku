#include <cmath>
#include "presentation/mapping/MapPresentationToCommand.h"

GridPosition MapPresentationToCommand::relativeToGrid(const glm::vec2 relativePos, const int boardSize) {
    int gridX = std::round((relativePos.x * (boardSize - 1)));
    int gridY = std::round((relativePos.y * (boardSize - 1)));
    return {gridX, gridY};
}

MouseCommandDTO MapPresentationToCommand::toMouseCommandDTO(const glm::vec2 relativePosition, const int boardSize) {
    return MouseCommandDTO(relativeToGrid(relativePosition, boardSize));
}

