#include "presentation/ui/BoardView.h"
#include "presentation/input/Input.h"
#include <iostream>

#include "logic/GameService.h"
#include "presentation/assets/AssetManager.h"
#include "presentation/mapping/MapPresentationToCommand.h"
#include "animator/SimpleAnimator.h"

BoardView::BoardView(std::string name, IGameService* gameService) : View(std::move(name)) {
    _gameService = gameService;
    const int size = _gameService->getBoardSize();
    _stoneSprites.resize(size, std::vector<Sprite*>(size, nullptr));
    _showHoverPreview = false;
    _mousePressed = false;
    _prevMousePressed = false;
    initializeSprites();
}

BoardView::~BoardView() {
    delete _backgroundBoard;
    delete _boardGrid;
    delete _hoverPreviewSprite;
    const int size = _gameService->getBoardSize();

    for (int y = 0; y < size; ++y) {
        for (int x = 0; x < size; ++x) {
            delete _stoneSprites[y][x];
        }
    }
}

void BoardView::initializeSprites() {
    auto* boardAnimator = new SimpleAnimator(.3, 0, "board");
    _backgroundBoard = new Sprite(AssetManager::getName(Textures::board_background),
                                 AssetManager::getName(Textures::board_background),
                                 glm::vec2(0, 0), glm::vec2(1, 1));
    _backgroundBoard->setLayer(0);
    _backgroundBoard->setVisible(true);
    _backgroundBoard->setAnimator(boardAnimator);
    addViewable(_backgroundBoard);

    _boardGrid = new Sprite(AssetManager::getName(Textures::board_grid),
                           AssetManager::getName(Textures::board_grid),
                           glm::vec2(0.06f, 0.06f), glm::vec2(0.88f, 0.88f));
    _boardGrid->setLayer(1);
    _boardGrid->setVisible(true);
    _boardGrid->setAnimator(boardAnimator);
    addViewable(_boardGrid);

    const float stoneSize = calculateStoneSize();

    _hoverPreviewSprite = new Sprite("hover_preview", AssetManager::getName(Textures::black_stone_hover), glm::vec2(0, 0), glm::vec2(stoneSize, stoneSize));
    _hoverPreviewSprite->setLayer(8);
    _hoverPreviewSprite->setVisible(false);
    addViewable(_hoverPreviewSprite);
}

void BoardView::render(Renderer* renderer, const glm::vec2 parentPos, const glm::vec2 parentDim) {
    View::render(renderer, parentPos, parentDim);
    handleMouseInput(renderer);
}

BoardViewDTO BoardView::getCurrentBoardState() const {
    return _gameService->getBoardState();
}

void BoardView::handleMouseInput(Renderer* renderer) {
    const glm::vec2 relativeMousePos = relativeInsideGridView(_boardGrid->getAbsPos(), _boardGrid->getAbsDim(), renderer->getCursorPos(), renderer->getViewportSize());
    handleMouseHover(relativeMousePos);
    handleMouseClick(relativeMousePos);
}

void BoardView::handleMouseHover(const glm::vec2 relativeMousePos) {
    const StoneViewDTO mouseHoverDTO = _gameService->processMouseHover(MapPresentationToCommand::toMouseCommandDTO(relativeMousePos, _gameService->getBoardSize()));
    if (!mouseHoverDTO.isValidPosition) {
        _showHoverPreview = false;
        _hoverPreviewSprite->setVisible(false);
        return;
    }
    _showHoverPreview = true;
    updateHoverPreview(mouseHoverDTO.previewColor, mouseHoverDTO.pos);
}

void BoardView::handleMouseClick(const glm::vec2 relativeMousePos) {
    _prevMousePressed = _mousePressed;
    _mousePressed = Input::get()->mousePressed(BUTTON_LEFT);

    if (_mousePressed && !_prevMousePressed) {
        const MoveViewDTO clickResult = _gameService->processMouseClick(MapPresentationToCommand::toMouseCommandDTO(relativeMousePos, _gameService->getBoardSize()));
        if (clickResult.success) {
            addStoneSprite(clickResult.stone, clickResult.boardView);
            _showHoverPreview = false;
            _hoverPreviewSprite->setVisible(false);
        }

        if (!clickResult.success) {
            std::cout << "Move failed: " << clickResult.errorMessage << std::endl;
        }
    }
}

void BoardView::addStoneSprite(const StoneViewDTO stone, const BoardViewDTO &boardView) {
    const std::string spriteName = "stone_" + std::to_string(stone.pos.x) + "_" + std::to_string(stone.pos.y);
    const glm::vec2 intersectionPos = gridToViewPosition(stone.pos, _boardGrid->getPos(), _boardGrid->getDim(), boardView.boardSize);
    const float stoneSize = calculateStoneSize();
    const glm::vec2 centeredPos = intersectionPos - glm::vec2(stoneSize * 0.5f);
    const auto stoneSprite = new Sprite(spriteName, getStoneTexture(stone.previewColor, false), centeredPos, glm::vec2(stoneSize, stoneSize));
    _stoneSprites[stone.pos.y][stone.pos.x] = stoneSprite;
    stoneSprite->setLayer(8);
    stoneSprite->setVisible(true);
    auto* animator = new SimpleAnimator(.3, stoneSprite->getLayer() + 1, "stone");
    stoneSprite->setAnimator(animator);
    addViewable(stoneSprite);

    if(!_backgroundBoard->getAnimator()->isActive())
        _backgroundBoard->getAnimator()->reset();
    animator->reset();
}

void BoardView::updateHoverPreview(const ViewColor previewColor, const ViewPosition hoverPosition) const {
    if (_showHoverPreview) {
        const int gridSize = _gameService->getBoardSize();
        const float stoneSize = calculateStoneSize();
        const glm::vec2 intersectionPos = gridToViewPosition(hoverPosition, _boardGrid->getPos(), _boardGrid->getDim(), gridSize);
        const glm::vec2 centeredPos = intersectionPos - glm::vec2(stoneSize * 0.5f);

        _hoverPreviewSprite->setPos(centeredPos);
        _hoverPreviewSprite->setDim(glm::vec2(stoneSize, stoneSize));
        _hoverPreviewSprite->setTexture(getStoneTexture(previewColor, true));
        _hoverPreviewSprite->setVisible(true);
    } else {
        _hoverPreviewSprite->setVisible(false);
    }
}

std::string BoardView::getStoneTexture(const ViewColor color, const bool isHover) {
    if (isHover) {
        return (color == ViewColor::BLACK) ? AssetManager::getName(Textures::black_stone_hover) : AssetManager::getName(Textures::white_stone_hover);
    }
    return (color == ViewColor::BLACK) ? AssetManager::getName(Textures::black_stone) : AssetManager::getName(Textures::white_stone);
}

float BoardView::calculateStoneSize() const {
    const float gridSpacing = 1.0f / (float)(_gameService->getBoardSize() - 1);
    return gridSpacing * _boardGrid->getDim().x;
}

glm::vec2 BoardView::gridToViewPosition(const ViewPosition position, const glm::vec2 boardPos, const glm::vec2 boardSize, const int boardSizeGrid) {
    const float gridSpacing = 1.0f / (float)(boardSizeGrid - 1);
    const auto relativeIntersection = glm::vec2(
        (float)position.x * gridSpacing,
        (float)position.y * gridSpacing
    );

    const glm::vec2 actualIntersection = boardPos + relativeIntersection * boardSize;
    return actualIntersection;
}

glm::vec2 BoardView::relativeInsideGridView(const glm::vec2 boardPos, const glm::vec2 boardSize,
                                                     const glm::vec2 mousePos, const glm::vec2 viewportSize) {
    const glm::vec2 normalizedMouse = mousePos / viewportSize;

    if (normalizedMouse.x < boardPos.x || normalizedMouse.x > boardPos.x + boardSize.x ||
        normalizedMouse.y < boardPos.y || normalizedMouse.y > boardPos.y + boardSize.y) {
        return {-1.0f, -1.0f};
    }

    // Convert to relative coordinates within the grid area (0.0 to 1.0)
    const glm::vec2 relativeMouse = (normalizedMouse - boardPos) / boardSize;

    return relativeMouse;
}