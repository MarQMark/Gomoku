#ifndef BOARDVIEW_H
#define BOARDVIEW_H

#include "presentation/ui/View.h"
#include "presentation/ui/Button.h"
#include "logic/GameService.h"
#include "logic/DTO/CommandDTOs.h"
#include "presentation/DTO/ViewModelDTOs.h"
#include <memory>
#include <array>

class BoardView : public View {
private:
    std::unique_ptr<GameService> _gameService;
    BoardViewDTO _currentBoardState;

    // 15x15 grid of buttons
    std::array<std::array<Button*, 15>, 15> _boardButtons;

    static constexpr int BOARD_SIZE = 15;

public:
    BoardView(std::string name);
    ~BoardView();

    void render(Renderer* renderer, glm::vec2 parentPos, glm::vec2 parentDim) override;

    // Board-specific methods
    const BoardViewDTO& getCurrentBoardState() const { return _currentBoardState; }

private:
    // Helper methods
    void initializeButtons();
    void updateButtonColors();
    void processMove(int x, int y);
    void updateBoardState(const MoveResultDTO& result);

    // Static callback for button clicks
    static void onButtonClicked(IInteractable* interactable, IInteractable::State state, void* data);

    // Helper struct to pass button position data
    struct ButtonData {
        BoardView* boardView;
        int x, y;
    };
};

#endif // BOARDVIEW_H