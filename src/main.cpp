#include "presentation/renderer/Renderer.h"

#define STB_IMAGE_IMPLEMENTATION
#include "presentation/renderer/stb_image/stb_image.h"
#include "presentation/ui/UI.h"
#include "presentation/ui/Button.h"
#include "presentation/ui/Label.h"
#include "presentation/ui/Sprite.h"
#include "presentation/ui/BoardView.h"

void newGameCallback(IInteractable* interactable, IInteractable::State state, void* data) {
    std::cout << "New Game Started!" << std::endl;
    // Cast data to BoardView and reset the game
    BoardView* boardView = static_cast<BoardView*>(data);
}

int main() {
    Renderer renderer;
    UI ui(&renderer);

    // Create the main game board (centered, square aspect ratio)
    auto* boardView = new BoardView("game_board");
    boardView->setKeepWidth(true);
    boardView->setPos(glm::vec2(0.1f, 0.1f));
    boardView->setDim(glm::vec2(0.5f, 0.5f));
    ui.getViewable<View>("root")->addViewable(boardView);

    // Create game status label
    auto* statusLabel = new Label("statusLabel", "Black Player's Turn");
    statusLabel->setPos(glm::vec2(0.75f, 0.85f));
    statusLabel->setDim(glm::vec2(0.2f, 0.05f));
    ui.getViewable<View>("root")->addViewable(statusLabel);

    // Create New Game button
    auto* newGameBtn = new Button("newGameBtn", glm::vec2(0.75f, 0.75f), glm::vec2(0.2f, 0.08f));
    newGameBtn->setKeepWidth(true);
    newGameBtn->registerCallback(newGameCallback, IInteractable::State::PRESSED, boardView);
    ui.getViewable<View>("root")->addViewable(newGameBtn);

    // Add labels to buttons
    auto* newGameLabel = new Label("newGameLabel", "New Game");
    newGameLabel->setPos(glm::vec2(0.01f, 0.02f));
    newGameLabel->setDim(glm::vec2(0.18f, 0.04f));
    ui.getViewable<View>("root")->addViewable(newGameLabel);

    while(renderer.shouldRun()) {
        // Update game status label based on current board state
        const auto& boardState = boardView->getCurrentBoardState();
        std::string statusText;

        switch(boardState.gameStatus) {
            case GameStatus::IN_PROGRESS:
                statusText = boardState.currentPlayerName + "'s Turn";
                break;
            case GameStatus::BLACK_WINS:
                statusText = "Black Wins!";
                break;
            case GameStatus::WHITE_WINS:
                statusText = "White Wins!";
                break;
            case GameStatus::DRAW:
                statusText = "Draw Game!";
                break;
            default:
                statusText = "Game Not Started";
                break;
        }

        statusLabel->setText(statusText);

        // Update UI and render
        ui.update();
        renderer.render();
    }

    // Cleanup
    delete boardView;
    delete statusLabel;
    delete newGameBtn;
    delete newGameLabel;
    return 0;
}