#include "presentation/ui/BoardView.h"
#include "presentation/input/Input.h"
#include "../../../include/logic/mapping/MapLogicToView.h"
#include <iostream>

#include "presentation/assets/AssetManager.h"
#include "presentation/mapping/MapPresentationToCommand.h"

BoardView::BoardView(std::string name, GameService* gameService) : View(std::move(name)) {
    _gameService = gameService;
    _currentBoardState = _gameService->getBoardState();

    _showHoverPreview = false;
    _mousePressed = false;
    _prevMousePressed = false;

    initializeSprites();
}

BoardView::~BoardView() {
    delete _backgroundBoard;
    delete _boardGrid;
    delete _hoverPreviewSprite;

    const int boardSize = GameService::getBoardSize();
    for (int y = 0; y < boardSize; ++y) {
        for (int x = 0; x < boardSize; ++x) {
            delete _stoneSprites[y][x];
        }
    }
}

void BoardView::initializeSprites() {
    // Background and grid setup
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

    const glm::vec2 boardPos = _boardGrid->getPos();
    const glm::vec2 boardSize = _boardGrid->getDim();

    // Get board size from logic layer
    const int gridSize = GameService::getBoardSize();
    const float stoneSize = calculateStoneSize(boardSize);

    // Create stone sprites using logic layer positioning
    for (int y = 0; y < gridSize; ++y) {
        for (int x = 0; x < gridSize; ++x) {
            const std::string spriteName = "stone_" + std::to_string(x) + "_" + std::to_string(y);

            // Get intersection position from logic layer
            glm::vec2 intersectionPos = gridToViewPosition(GridPosition(x, y), boardPos, boardSize, gridSize);
            const glm::vec2 centeredPos = intersectionPos - glm::vec2(stoneSize * 0.5f);

            _stoneSprites[y][x] = new Sprite(spriteName, AssetManager::GameTextures::BLACK_STONE,
                                           centeredPos, glm::vec2(stoneSize, stoneSize));
            _stoneSprites[y][x]->setLayer(6);
            _stoneSprites[y][x]->setVisible(false);
            addViewable(_stoneSprites[y][x]);
        }
    }

    _hoverPreviewSprite = new Sprite("hover_preview", AssetManager::GameTextures::BLACK_STONE_HOVER, glm::vec2(0, 0), glm::vec2(stoneSize, stoneSize));
    _hoverPreviewSprite->setLayer(8);
    _hoverPreviewSprite->setVisible(false);
    addViewable(_hoverPreviewSprite);
}

void BoardView::render(Renderer* renderer, const glm::vec2 parentPos, const glm::vec2 parentDim) {
    handleMouseInput(renderer);
    updateStoneDisplay();
    View::render(renderer, parentPos, parentDim);
}

void BoardView::handleMouseInput(Renderer* renderer) {
    const glm::vec2 relativeMousePos = relativeInsideGridView(_boardGrid->getAbsPos(), _boardGrid->getAbsDim(), renderer->getCursorPos(), renderer->getViewportSize());
    const MouseHoverViewDTO hoverResult = _gameService->processMouseHover(MapPresentationToCommand::toMouseCommandDTO(relativeMousePos));

    if (!hoverResult.isValidPosition) {
        _showHoverPreview = false;
        _hoverPreviewSprite->setVisible(false);
        return;
    }

    _showHoverPreview = true;
    updateHoverPreview(hoverResult.previewColor, hoverResult.stonePosition);

    // Handle clicks
    _prevMousePressed = _mousePressed;
    _mousePressed = Input::get()->mousePressed(BUTTON_LEFT);

    if (_mousePressed && !_prevMousePressed) {
        const MoveViewDTO clickResult = _gameService->processMouseClick(MapPresentationToCommand::toMouseCommandDTO(relativeMousePos));
        updateBoardState(clickResult);

        if (clickResult.success) {
            _showHoverPreview = false;
            _hoverPreviewSprite->setVisible(false);
        }

        if (!clickResult.success) {
            std::cout << "Move failed: " << clickResult.errorMessage << std::endl;
        }
    }
}

void BoardView::updateStoneDisplay() const {
    const int boardSize = GameService::getBoardSize();

    // Hide all stone sprites first
    for (int y = 0; y < boardSize; ++y) {
        for (int x = 0; x < boardSize; ++x) {
            _stoneSprites[y][x]->setVisible(false);
        }
    }

    // Show sprites for placed stones
    for (const auto& stone : _currentBoardState.stones) {
        Sprite* stoneSprite = _stoneSprites[stone.pos.y][stone.pos.x];
        stoneSprite->setTexture(getStoneTexture(stone.previewColor));
        stoneSprite->setVisible(true);
    }
}

void BoardView::updateHoverPreview(const StoneColor previewColor, const GridPosition hoverPosition) const {
    if (_showHoverPreview && hoverPosition.isValid()) {
        const int gridSize = GameService::getBoardSize();
        const float stoneSize = calculateStoneSize(_boardGrid->getDim());
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

void BoardView::updateBoardState(const MoveViewDTO& result) {
    _currentBoardState = result.boardState;
}

float BoardView::calculateStoneSize(const glm::vec2 boardSize) const {
    const float gridSpacing = 1.0f / (float)(_currentBoardState.boardSize - 1);
    return gridSpacing * boardSize.x;
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