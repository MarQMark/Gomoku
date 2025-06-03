#ifndef GOMOKU_STATSMENUVIEW_H
#define GOMOKU_STATSMENUVIEW_H


#include "presentation/ui/components/MenuView.h"
#include "MenuController.h"
#include "presentation/DTO/ViewModelDTOs.h"
#include "presentation/ui/components/Label.h"

class StatsMenuView : public MenuView {
public:
    StatsMenuView(std::string name, MenuController* menuController);
    ~StatsMenuView();

    void setBlackPlayer(std::string player);
    void setWhitePlayer(std::string player);
    void setGameTine(double gameTime);
    void setTurn(int turn);
    void setMove(int row, int column);
    void setGameStatus(GameStatus gameStatus);
    void setCurrentPlayer(const std::string &playerName);

    void setLastPlayerColor(ViewColor view_color);

private:
    Label* _text_lbl{};
    Label* _shadow_lbl{};

    std::string _black_player = "Player 1";
    std::string _white_player = "Player 2";
    std::string _current_player = "";
    std::string _last_player_color = "";
    GameStatus _game_status{};
    double _game_time = 0;
    int _turn = 0;
    int _move_row = 0;
    int _move_col = 0;

    void update_text();
    std::string format_time();
};


#endif //GOMOKU_STATSMENUVIEW_H
