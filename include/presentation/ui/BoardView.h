#ifndef BOARDVIEW_H
#define BOARDVIEW_H

#include "logic/interfaces/IBoardEventListener.h"
#include "presentation/ui/components/View.h"
#include "presentation/ui/components/Sprite.h"
#include "presentation/DTO/ViewModelDTOs.h"

class IGameService;

enum WinningLineOrientation {
    HORIZONTAL,
    VERTICAL,
    DIAGONAL_TOP_LEFT_TO_BOTTOM_RIGHT,
    DIAGONAL_BOTTOM_LEFT_TO_TOP_RIGHT
};

class BoardView final : public View, public IBoardEventListener {
    IGameService* _gameService;
    Sprite* _backgroundBoard{};
    Sprite* _boardGrid{};
    Sprite* _winningLine{};
    std::vector<std::vector<Sprite*>> _stoneSprites;
    Sprite* _hoverPreviewSprite{};

    bool _showHoverPreview;
    bool _mousePressed;
    bool _prevMousePressed;

public:
    explicit BoardView(std::string name, IGameService* gameService);
    ~BoardView() override;

    void render(Renderer* renderer, glm::vec2 parentPos, glm::vec2 parentDim) override;

    BoardViewDTO getCurrentBoardState() const;

private:
    void initializeSprites();

    void clearBoard();
    void updateHoverPreview(ViewColor previewColor, GridViewPosition hoverGridPosition) const;
    void handleMouseInput(Renderer* renderer);
    void handleMouseHover(glm::vec2 relativeMousePos);
    void addStoneSprite(const MoveViewDTO &move);
    void handleMouseClick(glm::vec2 relativeMousePos);
    static std::string getStoneTexture(ViewColor color, bool isHover = false);
    float calculateStoneSize() const;

    static glm::vec2 relativeInsideGridView(glm::vec2 boardPos, glm::vec2 boardSize,
                                             glm::vec2 mousePos, glm::vec2 viewportSize);

    static glm::vec2 gridToViewPosition(GridViewPosition position, glm::vec2 boardPos, glm::vec2 boardSize, int boardSizeGrid);

    static WinningLineOrientation determineWinningLineOrientation(const std::vector<GridViewPosition> &winningLine);

public:
    void onGameStarted() override;
    void onMoveCompleted(const MoveViewDTO &move) override;
    void onGameCompleted(GameCompleteViewDTO completeView) override;
};



#endif