#include <iostream>
#include "presentation/ui/MenuController.h"

#include "logic/interfaces/IGameService.h"
#include "presentation/DTO/ViewModelDTOs.h"
#include "presentation/ui/GameMenuView.h"
#include "presentation/ui/MenuButton.h"
#include "presentation/ui/StatsMenuView.h"

MenuController::MenuController(IGameService* gameService) {
    _game_service = gameService;
    _game_service->addMenuListener(this);
}

void MenuController::update() {
    bool inMotion = false;
    for (auto* menuView : _menu_views[_last_menu])
        inMotion = inMotion || menuView->inMotion();

    if(!inMotion){
        for (auto* menuView : _menu_views[_curr_menu])
            menuView->setHidden(false);
    }
}

void MenuController::addMenuView(Menu menu, MenuView *menuView) {
    _menu_views[menu].push_back(menuView);
}

void MenuController::changeMenu(Menu menu) {
    _last_menu = _curr_menu;
    for(auto* menuView : _menu_views[_last_menu])
        menuView->setHidden(true);
    _curr_menu = menu;
}

IGameService * MenuController::getGameService() const {
    return _game_service;
}

void MenuController::onStatsChanged(const StatsViewDTO stats) {
    const auto statsMenu = (StatsMenuView*)_menu_views[Menu::Game][1];
    statsMenu->setBlackPlayer(stats.blackPlayer);
    statsMenu->setWhitePlayer(stats.whitePlayer);
    statsMenu->setGameTine(stats.currentTime);
    statsMenu->setMove(stats.lastPosition.x, stats.lastPosition.y);
    statsMenu->setTurn(stats.turn);
    statsMenu->setGameStatus(stats.gameStatus);
    statsMenu->setCurrentPlayer(stats.currentPlayer);
}

GameMode MenuController::getSelectedGameMode() const {
    return _selected_game_mode;
}

void MenuController::setSelectedGameMode(const GameMode selected_game_mode) {
    _selected_game_mode = selected_game_mode;
}

void MenuController::onGameCompleted() {
    const auto gameMenuView = (GameMenuView*)_menu_views[Menu::Game][0];
    MenuButton* btn = gameMenuView->getViewable<MenuButton>("pauseBtn");
    btn->setText("Restart");
}
