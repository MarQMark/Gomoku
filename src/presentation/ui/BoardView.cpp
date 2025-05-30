#include "presentation/ui/BoardView.h"
#include "presentation/input/Input.h"
#include <iostream>

#include "presentation/assets/AssetManager.h"
#include "presentation/mapping/MapPresentationToCommand.h"

BoardView::BoardView(std::string name, GameService* gameService) : View(std::move(name)) {
    _gameService = gameService;
    _currentBoardState = _gameService->getBoardState();
    _stoneSprites.resize(_currentBoardState.boardSize, std::vector<Sprite*>(_currentBoardState.boardSize, nullptr));
    _showHoverPreview = false;
    _mousePressed = false;
    _prevMousePressed = false;
    initializeSprites();
}

BoardView::~BoardView() {
    delete _backgroundBoard;
    delete _boardGrid;
    delete _hoverPreviewSprite;

    for (int y = 0; y < _currentBoardState.boardSize; ++y) {
        for (int x = 0; x < _currentBoardState.boardSize; ++x) {
            delete _stoneSprites[y][x];
        }
    }
}

void BoardView::initializeSprites() {
    _backgroundBoard = new Sprite(AssetManager::GameTextures::BOARD_BACKGROUND,
                                 AssetManager::GameTextures::BOARD_BACKGROUND,
                                 glm::vec2(0, 0), glm::vec2(1, 1));
    _backgroundBoard->setLayer(0);
    _backgroundBoard->setVisible(true);
    addViewable(_backgroundBoard);

    _boardGrid = new Sprite(AssetManager::GameTextures::BOARD_GRID,
                           AssetManager::GameTextures::BOARD_GRID,
                           glm::vec2(0.06f, 0.06f), glm::vec2(0.88f, 0.88f));
    _boardGrid->setLayer(1);
    _boardGrid->setVisible(true);
    addViewable(_boardGrid);

    const float stoneSize = calculateStoneSize();

    _hoverPreviewSprite = new Sprite("hover_preview", AssetManager::GameTextures::BLACK_STONE_HOVER, glm::vec2(0, 0), glm::vec2(stoneSize, stoneSize));
    _hoverPreviewSprite->setLayer(8);
    _hoverPreviewSprite->setVisible(false);
    addViewable(_hoverPreviewSprite);
}

void BoardView::render(Renderer* renderer, const glm::vec2 parentPos, const glm::vec2 parentDim) {
    View::render(renderer, parentPos, parentDim);
    handleMouseInput(renderer);
}

BoardViewDTO BoardView::getCurrentBoardState() const {
    return _currentBoardState;
}


void BoardView::handleMouseInput(Renderer* renderer) {
    const glm::vec2 relativeMousePos = relativeInsideGridView(_boardGrid->getAbsPos(), _boardGrid->getAbsDim(), renderer->getCursorPos(), renderer->getViewportSize());
    handleMouseHover(relativeMousePos);
    handleMouseClick(relativeMousePos);
}

void BoardView::handleMouseHover(const glm::vec2 relativeMousePos) {
    const MouseHoverViewDTO mouseHoverDTO = _gameService->processMouseHover(MapPresentationToCommand::toMouseCommandDTO(relativeMousePos));
    if (!mouseHoverDTO.isValidPosition) {
        _showHoverPreview = false;
        _hoverPreviewSprite->setVisible(false);
        return;
    }
    _showHoverPreview = true;
    updateHoverPreview(mouseHoverDTO.previewColor, mouseHoverDTO.stonePosition);
}

void BoardView::handleMouseClick(const glm::vec2 relativeMousePos) {
    _prevMousePressed = _mousePressed;
    _mousePressed = Input::get()->mousePressed(BUTTON_LEFT);

    if (_mousePressed && !_prevMousePressed) {
        const MoveViewDTO clickResult = _gameService->processMouseClick(MapPresentationToCommand::toMouseCommandDTO(relativeMousePos));
        _currentBoardState = clickResult.boardView;
        if (clickResult.success) {
            addStoneSprite(clickResult.stone);
            _showHoverPreview = false;
            _hoverPreviewSprite->setVisible(false);
        }

        if (!clickResult.boardView.winningLine.empty()) {
            for (const auto pos : clickResult.boardView.winningLine) {
                std::cout << pos << std::endl;
            }
        }

        if (!clickResult.success) {
            std::cout << "Move failed: " << clickResult.errorMessage << std::endl;
        }
    }
}

void BoardView::addStoneSprite(const StoneViewDTO stone) {
    const std::string spriteName = "stone_" + std::to_string(stone.pos.x) + "_" + std::to_string(stone.pos.y);
    const glm::vec2 intersectionPos = gridToViewPosition(GridPosition(stone.pos.x, stone.pos.y), _boardGrid->getPos(), _boardGrid->getDim(), _currentBoardState.boardSize);
    const float stoneSize = calculateStoneSize();
    const glm::vec2 centeredPos = intersectionPos - glm::vec2(stoneSize * 0.5f);
    const auto stoneSprite = new Sprite(spriteName, getStoneTexture(stone.previewColor), centeredPos, glm::vec2(stoneSize, stoneSize));
    _stoneSprites[stone.pos.y][stone.pos.x] = stoneSprite;
    stoneSprite->setLayer(8);
    stoneSprite->setVisible(true);
    addViewable(stoneSprite);
}


void BoardView::updateHoverPreview(const StoneColor previewColor, const GridPosition hoverPosition) const {
    if (_showHoverPreview && hoverPosition.isValid()) {
        const int gridSize = GameService::getBoardSize();
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

std::string BoardView::getStoneTexture(const StoneColor color, const bool isHover) {
    if (isHover) {
        return (color == BLACK) ? AssetManager::GameTextures::BLACK_STONE_HOVER : AssetManager::GameTextures::WHITE_STONE_HOVER;
    }
    return (color == BLACK) ? AssetManager::GameTextures::BLACK_STONE : AssetManager::GameTextures::WHITE_STONE;
}


float BoardView::calculateStoneSize() const {
    const float gridSpacing = 1.0f / (float)(_currentBoardState.boardSize - 1);
    return gridSpacing * _boardGrid->getDim().x;
}


glm::vec2 BoardView::gridToViewPosition(const GridPosition position, const glm::vec2 boardPos, const glm::vec2 boardSize, const int boardSizeGrid) {
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