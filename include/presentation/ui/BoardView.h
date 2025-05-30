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

    std::vector<std::vector<Sprite*>> _stoneSprites;
    Sprite* _hoverPreviewSprite{};

    bool _showHoverPreview;
    bool _mousePressed;
    bool _prevMousePressed;

public:

    explicit BoardView(std::string name, GameService* gameService);
    ~BoardView() override;

    void render(Renderer* renderer, glm::vec2 parentPos, glm::vec2 parentDim) override;

    BoardViewDTO getCurrentBoardState() const;

private:
    void initializeSprites();
    void updateHoverPreview(StoneColor previewColor, GridPosition hoverPosition) const;
    void handleMouseInput(Renderer* renderer);
    void handleMouseHover(glm::vec2 relativeMousePos);
    void addStoneSprite(StoneViewDTO stone);
    void handleMouseClick(glm::vec2 relativeMousePos);
    static std::string getStoneTexture(StoneColor color, bool isHover = false);
    float calculateStoneSize() const;

    static glm::vec2 relativeInsideGridView(glm::vec2 boardPos, glm::vec2 boardSize,
                                             glm::vec2 mousePos, glm::vec2 viewportSize);

    static glm::vec2 gridToViewPosition(GridPosition position, glm::vec2 boardPos, glm::vec2 boardSize, int boardSizeGrid);
};

#endif