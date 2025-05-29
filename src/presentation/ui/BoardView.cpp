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

    _gridStart = glm::vec2(-0.01f, -0.01f);
    _gridSize = glm::vec2(1.0f, 1.0f);

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

    _boardGrid = new Sprite(AssetManager::GameTextures::BOARD_GRID, AssetManager::GameTextures::BOARD_GRID, glm::vec2(0.2, 0.2), glm::vec2(1, 1));
    _boardGrid->setVisible(false);
    addViewable(_boardGrid);

    // Create background board sprite
    _backgroundBoard = new Sprite(AssetManager::GameTextures::BOARD_BACKGROUND, AssetManager::GameTextures::BOARD_BACKGROUND, glm::vec2(0, 0), glm::vec2(1, 1));
    _backgroundBoard->setVisible(true);
    addViewable(_backgroundBoard);


    constexpr float gridSize = 1.0f / BOARD_SIZE;
    for (int y = 0; y < BOARD_SIZE; ++y) {
        for (int x = 0; x < BOARD_SIZE; ++x) {
            const std::string spriteName = "stone_" + std::to_string(x) + "_" + std::to_string(y);

            const glm::vec2 pos(x * gridSize, y * gridSize);
            constexpr glm::vec2 dim(gridSize, gridSize);

            _stoneSprites[y][x] = new Sprite(spriteName, AssetManager::GameTextures::BLACK_STONE, pos, dim);
            _stoneSprites[y][x]->setVisible(false);
            addViewable(_stoneSprites[y][x]);
        }
    }

    // Create hover preview sprite (HIDDEN)
    _hoverPreviewSprite = new Sprite("hover_preview", AssetManager::GameTextures::BLACK_STONE_HOVER, glm::vec2(0, 0), glm::vec2(gridSize, gridSize));
    _hoverPreviewSprite->setVisible(false);
    addViewable(_hoverPreviewSprite);
}

void BoardView::render(Renderer* renderer, const glm::vec2 parentPos, const glm::vec2 parentDim) {
    // Handle mouse input first
    handleMouseInput(renderer);
    // Update visual state
    updateStoneDisplay();
    updateHoverPreview();

    // Render all child elements
    View::render(renderer, parentPos, parentDim);
}

// Mouse to grid conversion using the grid overlay bounds
GridPosition BoardView::mouseToGrid(glm::vec2 mousePos) const {
    const glm::vec2 gridPos = _boardGrid->getAbsPos();
    const glm::vec2 gridSize = _boardGrid->getAbsDim();

    // Check if mouse is within grid
    if (mousePos.x < gridPos.x || mousePos.x > gridPos.x + gridSize.x ||
        mousePos.y < gridPos.y || mousePos.y > gridPos.y + gridSize.y) {
        return GridPosition(-1, -1);
        }

    // Convert to grid coordinates
    const glm::vec2 relativePos = (mousePos - gridPos) / gridSize;
    std::cout << relativePos.x << ", " << relativePos.y << std::endl;
    int gridX = static_cast<int>(relativePos.x * BOARD_SIZE);
    int gridY = static_cast<int>(relativePos.y * BOARD_SIZE);

    // Clamp
    gridX = std::max(0, std::min(BOARD_SIZE - 1, gridX));
    gridY = std::max(0, std::min(BOARD_SIZE - 1, gridY));

    return GridPosition(gridX, gridY);
}

glm::vec2 BoardView::gridToMouse(GridPosition gridPos, glm::vec2 viewPos, glm::vec2 viewSize) const {
    // Convert grid position to relative position within grid (center of cell)
    glm::vec2 relativeToGrid = glm::vec2(
        (gridPos.x + 0.5f) / BOARD_SIZE,  // +0.5 to center in cell
        (gridPos.y + 0.5f) / BOARD_SIZE
    );

    // Convert to relative position within view
    glm::vec2 relativeToView = _gridStart + relativeToGrid * _gridSize;

    // Convert to absolute screen position
    return viewPos + relativeToView * viewSize;
}

void BoardView::handleMouseInput(Renderer* renderer) {
    if (!isVisible()) return;

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

    std::cout << "Clicked on grid: (" << gridPos.x << ", " << gridPos.y << ")" << std::endl;

    // Create placement command
    PlaceStoneCommandDTO command(gridPos.x, gridPos.y,
                                (_currentBoardState.currentTurn == StoneColor::BLACK) ? "black_player" : "white_player");

    // Process through logic layer
    MoveResultDTO result = _gameService->processMove(command);
    updateBoardState(result);

    if (!result.success) {
        std::cout << "Move failed: " << result.errorMessage << std::endl;
    }
}

void BoardView::updateStoneDisplay() {
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

            // Set layer higher for last move or winning stones
            if (stone.isLastMove || stone.isWinningStone) {
                stoneSprite->setLayer(6); // Higher than normal stones
            } else {
                stoneSprite->setLayer(5);
            }
        }
    }
}

void BoardView::updateHoverPreview() {
    if (_showHoverPreview && _currentHoverPosition.isValid()) {
        // Position the hover preview sprite
        glm::vec2 screenPos = gridToScreenPosition(_currentHoverPosition, glm::vec2(0, 0), glm::vec2(1, 1));
        float gridSize = 1.0f / BOARD_SIZE;

        _hoverPreviewSprite->setPos(screenPos);
        _hoverPreviewSprite->setDim(glm::vec2(gridSize, gridSize));
        _hoverPreviewSprite->setTexture(getStoneTexture(_currentBoardState.currentTurn, true));
        _hoverPreviewSprite->setVisible(true);
        _hoverPreviewSprite->setLayer(7); // Above everything else
    } else {
        _hoverPreviewSprite->setVisible(false);
    }
}

// Helper methods
glm::vec2 BoardView::screenToRelative(glm::vec2 screenPos, glm::vec2 boardPos, glm::vec2 boardDim) {
    return (screenPos - boardPos) / boardDim;
}

glm::vec2 BoardView::gridToScreenPosition(GridPosition gridPos, glm::vec2 boardPos, glm::vec2 boardDim) {
    float gridSize = 1.0f / BOARD_SIZE;
    return boardPos + glm::vec2(gridPos.x * gridSize, gridPos.y * gridSize) * boardDim;
}

bool BoardView::isMouseInBoardArea(glm::vec2 mousePos, glm::vec2 boardPos, glm::vec2 boardDim) {
    return mousePos.x >= boardPos.x && mousePos.x <= boardPos.x + boardDim.x &&
           mousePos.y >= boardPos.y && mousePos.y <= boardPos.y + boardDim.y;
}

std::string BoardView::getStoneTexture(const StoneColor color, const bool isHover) {
    if (isHover) {
        return (color == StoneColor::BLACK) ? AssetManager::GameTextures::BLACK_STONE_HOVER : AssetManager::GameTextures::BLACK_STONE_HOVER;
    }
    return (color == StoneColor::BLACK) ? AssetManager::GameTextures::BLACK_STONE : AssetManager::GameTextures::WHITE_STONE;
}

void BoardView::updateBoardState(const MoveResultDTO& result) {
    _currentBoardState = result.boardState;
}