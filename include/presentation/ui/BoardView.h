#ifndef BOARDVIEW_H
#define BOARDVIEW_H

#include "logic/interfaces/IBoardEventListener.h"
#include "presentation/ui/components/View.h"
#include "presentation/ui/components/Sprite.h"
#include "presentation/DTO/ViewModelDTOs.h"

class IGameService;

enum WinningLineOrientation {
    HORIZONTAL,
    VERTICAL,
    DIAGONAL_TOP_LEFT_TO_BOTTOM_RIGHT,
    DIAGONAL_BOTTOM_LEFT_TO_TOP_RIGHT
};

class BoardView final : public View, public IBoardEventListener {
    IGameService* _game_service{};
    Sprite* _background_board{};
    Sprite* _board_grid{};
    Sprite* _winning_line{};
    std::vector<std::vector<Sprite*>> _stone_sprites;
    Sprite* _hover_preview_sprite{};

    bool _show_hover_preview;
    bool _mouse_pressed;
    bool _prev_mouse_pressed;

public:
    explicit BoardView(std::string name, IGameService* gameService);
    ~BoardView() override;

    void render(Renderer* renderer, glm::vec2 parentPos, glm::vec2 parentDim) override;

    BoardViewDTO getCurrentBoardState() const;

private:
    void initialize_sprites();

    void clear_board();
    void update_hover_preview(ViewColor previewColor, GridViewPosition hoverGridPosition) const;
    void handle_mouse_input(Renderer* renderer);
    void handle_mouse_hover(glm::vec2 relativeMousePos);
    void add_stone_sprite(const MoveViewDTO &move);
    void handle_mouse_click(glm::vec2 relativeMousePos);
    static std::string get_stone_texture(ViewColor color, bool isHover = false);
    float calculate_stone_size() const;

    static glm::vec2 relative_inside_grid_view(glm::vec2 boardPos, glm::vec2 boardSize,
                                               glm::vec2 mousePos, glm::vec2 viewportSize);

    static glm::vec2 grid_to_view_position(GridViewPosition position, glm::vec2 boardPos, glm::vec2 boardSize, int boardSizeGrid);

    static WinningLineOrientation determine_winning_line_orientation(const std::vector<GridViewPosition> &winningLine);

public:
    void onGameStarted() override;
    void onMoveCompleted(const MoveViewDTO &move) override;
    void onGameCompleted(GameCompleteViewDTO completeView) override;
};



#endif