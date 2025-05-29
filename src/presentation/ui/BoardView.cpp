#include "presentation/ui/BoardView.h"
#include "presentation/input/Input.h"
#include <iostream>

#include "presentation/assets/AssetManager.h"

BoardView::BoardView(std::string name) : View(std::move(name)) {
    _gameService = std::make_unique<GameService>();

    // Initialize state
    _currentBoardState.boardSize = 15;
    _currentBoardState.currentTurn = StoneColor::BLACK;
    _currentBoardState.currentPlayerName = "Black Player";
    _currentBoardState.gameStatus = GameStatus::IN_PROGRESS;
    _currentBoardState.moveNumber = 0;

    _currentHoverPosition = GridPosition(-1, -1);
    _showHoverPreview = false;
    _mousePressed = false;
    _prevMousePressed = false;

    initializeSprites();
}

BoardView::~BoardView() {
    delete _backgroundBoard;
    delete _boardGrid;
    delete _hoverPreviewSprite;

    for (int y = 0; y < BOARD_SIZE; ++y) {
        for (int x = 0; x < BOARD_SIZE; ++x) {
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

    glm::vec2 gridPos = _boardGrid->getPos();
    glm::vec2 gridSize = _boardGrid->getDim();

    float gridSpacing = 1.0f / (BOARD_SIZE - 1);
    float stoneSize = gridSpacing * gridSize.x;

    for (int y = 0; y < BOARD_SIZE; ++y) {
        for (int x = 0; x < BOARD_SIZE; ++x) {
            const std::string spriteName = "stone_" + std::to_string(x) + "_" + std::to_string(y);

            // Calculate intersection position WITHIN the grid area
            glm::vec2 relativeIntersection = glm::vec2(x * gridSpacing, y * gridSpacing);
            glm::vec2 actualIntersection = gridPos + relativeIntersection * gridSize;

            // Center stone sprite on intersection
            glm::vec2 centeredPos = actualIntersection - glm::vec2(stoneSize * 0.5f);

            _stoneSprites[y][x] = new Sprite(spriteName, AssetManager::GameTextures::BLACK_STONE,
                                           centeredPos, glm::vec2(stoneSize, stoneSize));
            _stoneSprites[y][x]->setLayer(6);
            _stoneSprites[y][x]->setVisible(true);
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
    updateHoverPreview();
    updateStoneDisplay();
    View::render(renderer, parentPos, parentDim);
}

// Mouse to grid conversion using the grid overlay bounds
GridPosition BoardView::mouseToGrid(glm::vec2 mousePos) const {
    const glm::vec2 gridPos = _boardGrid->getAbsPos();
    const glm::vec2 gridSize = _boardGrid->getAbsDim();

    // Mouse not in grid
    if (mousePos.x < gridPos.x || mousePos.x > gridPos.x + gridSize.x ||
        mousePos.y < gridPos.y || mousePos.y > gridPos.y + gridSize.y)
        return GridPosition(-1, -1);

    // Convert to grid coordinates
    const glm::vec2 relativePos = (mousePos - gridPos) / gridSize;
    int gridX = static_cast<int>(relativePos.x * (BOARD_SIZE - 1) + 0.5f);
    int gridY = static_cast<int>(relativePos.y * (BOARD_SIZE - 1) + 0.5f);

    gridX = std::max(0, std::min(BOARD_SIZE - 1, gridX));
    gridY = std::max(0, std::min(BOARD_SIZE - 1, gridY));

    return GridPosition(gridX, gridY);
}

void BoardView::handleMouseInput(Renderer* renderer) {
    glm::vec2 mousePos = renderer->getCursorPos();
    glm::vec2 viewportSize = renderer->getViewportSize();
    mousePos = mousePos / viewportSize; // Normalize to 0-1

    // Use the actual absolute position and dimensions of this view
    GridPosition gridPos = mouseToGrid(mousePos);

    if (gridPos.x >= 0 && gridPos.y >= 0) {
        processHover(gridPos);

        // Handle clicks
        _prevMousePressed = _mousePressed;
        _mousePressed = Input::get()->mousePressed(Mouse::BUTTON_LEFT);

        if (_mousePressed && !_prevMousePressed) {
            processClick(gridPos);
        }
    } else {
        // Mouse is outside grid
        _showHoverPreview = false;
    }
}

void BoardView::processHover(GridPosition gridPos) {
    if (_currentBoardState.gameStatus != GameStatus::IN_PROGRESS) {
        _showHoverPreview = false;
        return;
    }

    // Check if position is occupied
    bool occupied = false;
    for (const auto& stone : _currentBoardState.stones) {
        if (stone.x == gridPos.x && stone.y == gridPos.y) {
            occupied = true;
            break;
        }
    }

    if (!occupied) {
        _currentHoverPosition = gridPos;
        _showHoverPreview = true;
    } else {
        _showHoverPreview = false;
        _currentHoverPosition = GridPosition(-1, -1);
        _hoverPreviewSprite->setVisible(false);
    }
}


void BoardView::processClick(GridPosition gridPos) {
    if (_currentBoardState.gameStatus != GameStatus::IN_PROGRESS) {
        return;
    }

    // Check if position is available
    for (const auto& stone : _currentBoardState.stones) {
        if (stone.x == gridPos.x && stone.y == gridPos.y) {
            std::cout << "Position already occupied!" << std::endl;
            return;
        }
    }

    const PlaceStoneCommandDTO command(gridPos.x, gridPos.y,
                                     (_currentBoardState.currentTurn == StoneColor::BLACK) ? "black_player" : "white_player");
    const MoveResultDTO result = _gameService->processMove(command);
    updateBoardState(result);
    _showHoverPreview = false;
    _hoverPreviewSprite->setVisible(false);

    if (!result.success) {
        std::cout << "Move failed: " << result.errorMessage << std::endl;
    }
}

void BoardView::updateStoneDisplay() const {
    // Hide all stone sprites first
    for (int y = 0; y < BOARD_SIZE; ++y) {
        for (int x = 0; x < BOARD_SIZE; ++x) {
            _stoneSprites[y][x]->setVisible(false);
        }
    }

    // Show sprites for placed stones
    for (const auto& stone : _currentBoardState.stones) {
        if (stone.x >= 0 && stone.x < BOARD_SIZE && stone.y >= 0 && stone.y < BOARD_SIZE) {
            Sprite* stoneSprite = _stoneSprites[stone.y][stone.x];
            stoneSprite->setTexture(getStoneTexture(stone.color));
            stoneSprite->setVisible(true);
        }
    }
}

void BoardView::updateHoverPreview() const {
    if (_showHoverPreview && _currentHoverPosition.isValid()) {
        glm::vec2 gridPos = _boardGrid->getPos();
        glm::vec2 gridSize = _boardGrid->getDim();

        float gridSpacing = 1.0f / (BOARD_SIZE - 1);
        float stoneSize = gridSpacing * gridSize.x;

        // Calculate intersection position WITHIN the grid area
        glm::vec2 relativeIntersection = glm::vec2(
            static_cast<float>(_currentHoverPosition.x) * gridSpacing,
            static_cast<float>(_currentHoverPosition.y) * gridSpacing
        );
        glm::vec2 actualIntersection = gridPos + relativeIntersection * gridSize;

        // Center stone sprite on intersection
        glm::vec2 centeredPos = actualIntersection - glm::vec2(stoneSize * 0.5f);

        _hoverPreviewSprite->setPos(centeredPos);
        _hoverPreviewSprite->setDim(glm::vec2(stoneSize, stoneSize));
        _hoverPreviewSprite->setTexture(getStoneTexture(_currentBoardState.currentTurn, true));
        _hoverPreviewSprite->setVisible(true);
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