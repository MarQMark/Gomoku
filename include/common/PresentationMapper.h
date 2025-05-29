#ifndef PRESENTATIONMAPPER_H
#define PRESENTATIONMAPPER_H

#include "logic/Board.h"
#include "logic/DTO/CommandDTOs.h"
#include "presentation/DTO/ViewModelDTOs.h"
#include <vector>

#include "glm/vec2.hpp"

class PresentationMapper {
public:
    // === Presentation → Logic ===
    static GridPosition commandToPosition(const PlaceStoneCommandDTO& cmd);
    static bool validatePlaceStoneCommand(const PlaceStoneCommandDTO& cmd);
    static bool validateMouseCommand(const MouseCommandDTO& cmd);

    // === Logic → Presentation ===
    static BoardViewDTO toBoardView(const Board& board, const GameState& state, const std::vector<GridPosition>& winningLine = {});
    static MoveResultDTO createMoveResult(bool success, const Board& board, const GameState& state, const std::string& error = "", const std::vector<GridPosition>& winningLine = {});

    // === Coordinate conversion helpers ===
    static glm::vec2 boardAreaToGridRelative(glm::vec2 boardPos, glm::vec2 boardSize,
                                           glm::vec2 mousePos, glm::vec2 viewportSize);

    // Convert grid coordinates to view coordinates within board area
    static glm::vec2 gridToViewPosition(int gridX, int gridY, glm::vec2 boardPos, glm::vec2 boardSize, int boardSizeGrid);

    // Calculate stone size based on board dimensions
    static float calculateStoneSize(glm::vec2 boardSize, int boardSizeGrid);
};

#endif