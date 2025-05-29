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
    std::unique_ptr<GameService> _gameService;
    BoardViewDTO _currentBoardState;

    Sprite* _backgroundBoard{};
    Sprite* _boardGrid{};
    std::array<std::array<Sprite*, Board::SIZE>, Board::SIZE> _stoneSprites{};
    Sprite* _hoverPreviewSprite{};

    GridPosition _currentHoverPosition;
    bool _showHoverPreview;
    bool _mousePressed;
    bool _prevMousePressed;

public:
    explicit BoardView(std::string name);
    ~BoardView() override;

    void render(Renderer* renderer, glm::vec2 parentPos, glm::vec2 parentDim) override;

    const BoardViewDTO& getCurrentBoardState() const { return _currentBoardState; }

private:
    void initializeSprites();
    void updateStoneDisplay() const;
    void updateHoverPreview(StoneColor previewColor) const;

    void handleMouseInput(Renderer* renderer);

    static std::string getStoneTexture(StoneColor color, bool isHover = false);
    void updateBoardState(const MoveResultDTO& result);
};

#endif