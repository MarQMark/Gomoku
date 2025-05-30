#ifndef BOARDVIEW_H
#define BOARDVIEW_H

#include "presentation/ui/View.h"
#include "presentation/ui/Sprite.h"
#include "logic/GameService.h"
#include "presentation/DTO/ViewModelDTOs.h"
#include <memory>
#include <array>

class BoardView final : public View {
private:
    GameService* _gameService;
    BoardViewDTO _currentBoardState;

    Sprite* _backgroundBoard{};
    Sprite* _boardGrid{};
    std::array<std::array<Sprite*, Board::SIZE>, Board::SIZE> _stoneSprites{};
    Sprite* _hoverPreviewSprite{};

    bool _showHoverPreview;
    bool _mousePressed;
    bool _prevMousePressed;

public:

    explicit BoardView(std::string name, GameService* gameService);
    ~BoardView() override;

    void render(Renderer* renderer, glm::vec2 parentPos, glm::vec2 parentDim) override;

    const BoardViewDTO& getCurrentBoardState() const { return _currentBoardState; }

private:
    void initializeSprites();
    void updateStoneDisplay() const;
    void updateHoverPreview(StoneColor previewColor, GridPosition hoverPosition) const;

    void handleMouseInput(Renderer* renderer);

    static std::string getStoneTexture(StoneColor color, bool isHover = false);
    void updateBoardState(const MoveViewDTO& result);

    float calculateStoneSize(glm::vec2 boardSize) const;

    static glm::vec2 relativeInsideGridView(glm::vec2 boardPos, glm::vec2 boardSize,
                                             glm::vec2 mousePos, glm::vec2 viewportSize);

    static glm::vec2 gridToViewPosition(GridPosition position, glm::vec2 boardPos, glm::vec2 boardSize, int boardSizeGrid);
};

#endif