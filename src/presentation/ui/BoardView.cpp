#include "presentation/ui/BoardView.h"
#include "presentation/input/Input.h"
#include "common/PresentationMapper.h"
#include <iostream>

#include "presentation/assets/AssetManager.h"

BoardView::BoardView(std::string name) : View(std::move(name)) {
    _gameService = std::make_unique<GameService>();

    // Initialize state from GameService
    const GameState& gameState = _gameService->getCurrentState();
    _currentBoardState = PresentationMapper::toBoardView(gameState.board, gameState);

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
    const float stoneSize = PresentationMapper::calculateStoneSize(boardSize, gridSize);

    // Create stone sprites using logic layer positioning
    for (int y = 0; y < gridSize; ++y) {
        for (int x = 0; x < gridSize; ++x) {
            const std::string spriteName = "stone_" + std::to_string(x) + "_" + std::to_string(y);

            // Get intersection position from logic layer
            glm::vec2 intersectionPos = PresentationMapper::gridToViewPosition(x, y, boardPos, boardSize, gridSize);
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
    const std::string currentPlayerId = _gameService->getCurrentPlayerId();
    const glm::vec2 relativeMousePos = PresentationMapper::boardAreaToGridRelative(_boardGrid->getAbsPos(), _boardGrid->getAbsDim(), renderer->getCursorPos(), renderer->getViewportSize());
    const GridHoverResultDTO hoverResult = _gameService->processMouseHover(PresentationMapper::toMouseCommandDTO(relativeMousePos, currentPlayerId));

    if (hoverResult.isValidPosition) {
        _currentHoverPosition = hoverResult.gridPosition;
        _showHoverPreview = true;
        updateHoverPreview(hoverResult.previewColor);

        // Handle clicks
        _prevMousePressed = _mousePressed;
        _mousePressed = Input::get()->mousePressed(Mouse::BUTTON_LEFT);

        if (_mousePressed && !_prevMousePressed) {
            const MoveResultDTO clickResult = _gameService->processMouseClick(PresentationMapper::toMouseCommandDTO(relativeMousePos, currentPlayerId));
            updateBoardState(clickResult);

            if (clickResult.success) {
                _showHoverPreview = false;
                _hoverPreviewSprite->setVisible(false);
            }

            if (!clickResult.success) {
                std::cout << "Move failed: " << clickResult.errorMessage << std::endl;
            }
        }
    } else {
        // Mouse is outside grid area or position invalid
        _showHoverPreview = false;
        _hoverPreviewSprite->setVisible(false);
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
        if (stone.x >= 0 && stone.x < boardSize && stone.y >= 0 && stone.y < boardSize) {
            Sprite* stoneSprite = _stoneSprites[stone.y][stone.x];
            stoneSprite->setTexture(getStoneTexture(stone.color));
            stoneSprite->setVisible(true);
        }
    }
}

void BoardView::updateHoverPreview(const StoneColor previewColor) const {
    if (_showHoverPreview && _currentHoverPosition.isValid()) {
        const glm::vec2 boardPos = _boardGrid->getPos();
        const glm::vec2 boardSize = _boardGrid->getDim();
        const int gridSize = GameService::getBoardSize();
        const float stoneSize = PresentationMapper::calculateStoneSize(boardSize, gridSize);
        const glm::vec2 intersectionPos = PresentationMapper::gridToViewPosition(_currentHoverPosition.x, _currentHoverPosition.y, boardPos, boardSize, gridSize);
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
        return (color == StoneColor::BLACK) ? AssetManager::GameTextures::BLACK_STONE_HOVER : AssetManager::GameTextures::WHITE_STONE_HOVER;
    }
    return (color == StoneColor::BLACK) ? AssetManager::GameTextures::BLACK_STONE : AssetManager::GameTextures::WHITE_STONE;
}

void BoardView::updateBoardState(const MoveResultDTO& result) {
    _currentBoardState = result.boardState;
}