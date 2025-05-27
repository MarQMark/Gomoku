#include "presentation/ui/BoardView.h"
#include <iostream>

BoardView::BoardView(std::string name) : View(std::move(name)) {
    _gameService = std::make_unique<GameService>();

    // Initialize with empty board state
    _currentBoardState.boardSize = 15;
    _currentBoardState.currentTurn = StoneColor::BLACK;
    _currentBoardState.currentPlayerName = "Black Player";
    _currentBoardState.gameStatus = GameStatus::IN_PROGRESS;
    _currentBoardState.moveNumber = 0;

    initializeButtons();
}

BoardView::~BoardView() {
}

void BoardView::initializeButtons() {
    float spacing = 0.001;
    float gridSize = 1.0f / BOARD_SIZE;
    float buttonSize = gridSize - 2 * spacing;

    for (int y = 0; y < BOARD_SIZE; ++y) {
        for (int x = 0; x < BOARD_SIZE; ++x) {
            const std::string buttonName = "btn_" + std::to_string(x) + "_" + std::to_string(y);

            // Calculate position and size
            const glm::vec2 pos(x * gridSize + spacing, y * gridSize + spacing);
            const glm::vec2 dim(buttonSize, buttonSize);

            const auto button = new Button(buttonName, pos, dim);
            const auto data = new ButtonData{this, x, y};

            button->registerCallback(onButtonClicked, IInteractable::State::RELEASED, data);
            _boardButtons[y][x] = button;
            addViewable(button);
        }
    }
}

void BoardView::render(Renderer* renderer, glm::vec2 parentPos, glm::vec2 parentDim) {
   // updateButtonColors();
    View::render(renderer, parentPos, parentDim);
}

void BoardView::updateButtonColors() {
}

void BoardView::onButtonClicked(IInteractable* interactable, IInteractable::State state, void* data) {
    if (state == IInteractable::State::RELEASED && data) {
        const ButtonData* buttonData = static_cast<ButtonData*>(data);
        BoardView* boardView = buttonData->boardView;

        std::cout << "Button clicked at: " << buttonData->x << ", " << buttonData->y << std::endl;

        // Check if position is already occupied
        bool occupied = false;
        for (const auto& stone : boardView->_currentBoardState.stones) {
            if (stone.x == buttonData->x && stone.y == buttonData->y) {
                occupied = true;
                break;
            }
        }

        if (!occupied) {
            boardView->processMove(buttonData->x, buttonData->y);
        } else {
            std::cout << "Position already occupied!" << std::endl;
        }
    }
}

void BoardView::processMove(int x, int y) {
    // Presentation -> Logic
    PlaceStoneCommandDTO command;
    command.x = x;
    command.y = y;
    command.playerId = (_currentBoardState.currentTurn == StoneColor::BLACK) ? "black_player" : "white_player";

    // Process through Logic layer
    MoveResultDTO result = _gameService->processMove(command);

    // Update UI state based on result
    updateBoardState(result);

    if (!result.success) {
        std::cout << "Move failed: " << result.errorMessage << std::endl;
    } else {
        std::cout << "Move successful! Turn: " <<
                     (result.boardState.currentTurn == StoneColor::BLACK ? "Black" : "White") << std::endl;

        if (result.winner.has_value()) {
            std::cout << "Game Over! Winner: " <<
                         (result.winner.value() == StoneColor::BLACK ? "Black" : "White") << std::endl;
        }
    }
}

void BoardView::updateBoardState(const MoveResultDTO& result) {
    _currentBoardState = result.boardState;
}