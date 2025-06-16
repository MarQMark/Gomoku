#include <sstream>
#include <iostream>
#include "presentation/ui/StatsMenuView.h"
#include "presentation/ui/components/Sprite.h"
#include "presentation/assets/AssetManager.h"
#include "presentation/ui/components/IViewable.h"

StatsMenuView::StatsMenuView(std::string name, MenuController *menuController) : MenuView(std::move(name)){
    menuController->addMenuView(MenuController::Game, this);

    MenuView::setVisible(false);
    MenuView::setHidden(true);
    MenuView::setPos(glm::vec2(.75, 0));
    MenuView::setDim(glm::vec2(.25, .7));
    MenuView::setLeft(false);
    MenuView::setAlignH(Align::CENTER);
    MenuView::setMargin(0.0, 0, 0.01, 0);
    // MenuView::setAlignH(IViewable::Align::TOP);

    Sprite* background = new Sprite("statsBackground", AssetManager::getName(Textures::stats_background));
    background->setKeepHeight(true);
    background->setDim(glm::vec2(1.75, 1));
    MenuView::addViewable(background);

    _text_lbl = new Label("statsBlack", "");
    _text_lbl->setPos(glm::vec2(.1, 0.030));
    _text_lbl->setDim(glm::vec2(0, .15 * .22));
    MenuView::addViewable(_text_lbl);

    _shadow_lbl = new Label("statsShadow", "");
    _shadow_lbl->setPos(glm::vec2(.1, .037));
    _shadow_lbl->setDim(glm::vec2(0, .15 * .22));
    _shadow_lbl->setFontColor(glm::vec4(0.082, 0.114, 0.157, 1));
    _shadow_lbl->setLayer(_text_lbl->getLayer() - 0.01);
    MenuView::addViewable(_shadow_lbl);

    update_text();
}

StatsMenuView::~StatsMenuView() {
    deleteViewable<Sprite>("statsBackground");
    deleteViewable<Label>("statsBlack");
    deleteViewable<Label>("statsShadow");
}

void StatsMenuView::update_text() {
    std::stringstream ss;
    ss << "\n\n\nBlack:";
    ss << " " << _black_player << "\n\n";
    ss << "White:";
    ss << " " << _white_player << "\n\n";
    ss << "Time: " << format_time() << "\n\n";
    ss << "Turn: " << _turn << "\n\n";
    if (_move_col >= 1 && _move_row >= 1) {
        ss << "Last Turn: \n" ;
        ss << " " << _last_player_color << " at: (" << _move_col << "," << _move_row << ")\n";
    }
    ss << "\n";
    if (_game_status == BLACK_WINS)
        ss << "> " << _black_player << " wins!\n";
    if (_game_status == WHITE_WINS)
        ss << "> " << _white_player << " wins!\n";
    if (_game_status == DRAW)
        ss << "> " << "Game Draw!\n";
    if (_game_status == PAUSED)
        ss << "> " << "Game Paused!\n";
    if (_game_status == IN_PROGRESS) {
        ss << "> " << _current_player << " turn!\n";
    }

    _text_lbl->setText(ss.str());
    _shadow_lbl->setText(ss.str());
}

std::string StatsMenuView::format_time() {
    std::stringstream ss;
    int min = _game_time / 60;
    if(min > 0)
        ss << min << "m ";
    ss<< (int)_game_time % 60 << "s";
    return ss.str();
}

void StatsMenuView::setBlackPlayer(std::string player) {
    _black_player = player;
    update_text();
}

void StatsMenuView::setWhitePlayer(std::string player) {
    _white_player = player;
    update_text();
}

void StatsMenuView::setGameTine(const double gameTime) {
    _game_time = gameTime;
    update_text();
}

void StatsMenuView::setTurn(const int turn) {
    _turn = turn;
    update_text();
}

void StatsMenuView::setMove(const int row, const int column) {
    _move_row = row;
    _move_col = column;
    update_text();
}

void StatsMenuView::setGameStatus(const GameStatus gameStatus) {
    _game_status = gameStatus;
    update_text();
}

void StatsMenuView::setCurrentPlayer(const std::string &playerName) {
    _current_player = playerName;
    update_text();
}

void StatsMenuView::setLastPlayerColor(const ViewColor viewColor) {
    if (viewColor == ViewColor::BLACK)
        _last_player_color = "Black";
    else
        _last_player_color = "White";
    update_text();
}
