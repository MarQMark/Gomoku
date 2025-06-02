#include <sstream>
#include <iostream>
#include "presentation/ui/StatsMenuView.h"
#include "presentation/ui/components/Sprite.h"
#include "presentation/assets/AssetManager.h"

StatsMenuView::StatsMenuView(std::string name, MenuController *menuController) : MenuView(std::move(name)){
    menuController->addMenuView(MenuController::Game, this);

    MenuView::setVisible(false);
    MenuView::setHidden(true);
    MenuView::setPos(glm::vec2(.75, 0));
    MenuView::setDim(glm::vec2(.25, .7));
    MenuView::setLeft(false);
    MenuView::setMargin(0.15, 0, 0.01, 0);
    MenuView::setAlignH(IViewable::Align::TOP);

    Sprite* background = new Sprite("statsBackground", AssetManager::getName(Textures::stats_background));
    background->setKeepHeight(true);
    background->setDim(glm::vec2(1.75, 1));
    MenuView::addViewable(background);

    _text_lbl = new Label("statsBlack", "");
    _text_lbl->setPos(glm::vec2(.1, 0));
    _text_lbl->setDim(glm::vec2(0, .15 * .28));
    MenuView::addViewable(_text_lbl);

    _shadow_lbl = new Label("statsShadow", "");
    _shadow_lbl->setPos(glm::vec2(.1, .008));
    _shadow_lbl->setDim(glm::vec2(0, .15 * .28));
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
    ss << "\nBlack:\n";
    ss << "  " << _black_player << "\n\n";
    ss << "White:\n";
    ss << "  " << _white_player << "\n\n";
    ss << "Time: " << format_time() << "\n\n";
    ss << "Turn: " << _turn << "\n\n";
    ss << "Last Move:\n";
    ss << "   Column: "<< _move_col << "\n";
    ss << "   Row: " << _move_row << "\n";

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

void StatsMenuView::setGameTine(double gameTime) {
    _game_time = gameTime;
    update_text();
}

void StatsMenuView::setTurn(int turn) {
    _turn = turn;
    update_text();
}

void StatsMenuView::setMove(int row, int column) {
    _move_row = row;
    _move_col = column;
    update_text();
}
