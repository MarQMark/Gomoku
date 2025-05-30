#include "presentation/mapping/MapPresentationToCommand.h"

MouseCommandDTO MapPresentationToCommand::toMouseCommandDTO(glm::vec2 relativePosition) {
    return MouseCommandDTO(relativeToGrid(relativePosition));
}

GridPosition MapPresentationToCommand::relativeToGrid(const glm::vec2 relativePos) {
    int gridX = static_cast<int>(relativePos.x * (Board::SIZE - 1) + 0.5f);
    int gridY = static_cast<int>(relativePos.y * (Board::SIZE - 1) + 0.5f);
    return {gridX, gridY};
}

