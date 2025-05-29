#ifndef BOARDVIEW_H
#define BOARDVIEW_H

#include "presentation/ui/View.h"
#include "presentation/ui/Sprite.h"
#include "logic/GameService.h"
#include "logic/DTO/CommandDTOs.h"
#include "presentation/DTO/ViewModelDTOs.h"
#include <memory>
#include <array>

class BoardView final : public View {
private:
    std::unique_ptr<GameService> _gameService;
    BoardViewDTO _currentBoardState;

    // Visual elements
    Sprite* _backgroundBoard;
    Sprite* _boardGrid;
    std::array<std::array<Sprite*, 15>, 15> _stoneSprites;
    Sprite* _hoverPreviewSprite;

    // State tracking
    GridPosition _currentHoverPosition;
    bool _showHoverPreview;
    bool _mousePressed;
    bool _prevMousePressed;

    static constexpr int BOARD_SIZE = 15;

    glm::vec2 _gridStart;
    glm::vec2 _gridSize;

public:
    BoardView(std::string name);
    ~BoardView();

    void render(Renderer* renderer, glm::vec2 parentPos, glm::vec2 parentDim) override;

    // Board state access
    const BoardViewDTO& getCurrentBoardState() const { return _currentBoardState; }

private:
    // Initialization
    void initializeSprites();
    void updateStoneDisplay();
    void updateHoverPreview();

    // Mouse interaction
    void handleMouseInput(Renderer* renderer);
    void processHover(GridPosition mousePos);
    void processClick(GridPosition mousePos);

    // Simple coordinate conversion
    GridPosition mouseToGrid(glm::vec2 mousePos) const;
    glm::vec2 gridToMouse(GridPosition gridPos, glm::vec2 viewPos, glm::vec2 viewSize) const;

    // Coordinate conversion
    glm::vec2 screenToRelative(glm::vec2 screenPos, glm::vec2 boardPos, glm::vec2 boardDim);
    glm::vec2 gridToScreenPosition(GridPosition gridPos, glm::vec2 boardPos, glm::vec2 boardDim);

    // Helper methods
    bool isMouseInBoardArea(glm::vec2 mousePos, glm::vec2 boardPos, glm::vec2 boardDim);
    std::string getStoneTexture(StoneColor color, bool isHover = false);
    void updateBoardState(const MoveResultDTO& result);
};

#endif