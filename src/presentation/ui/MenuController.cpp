#include <iostream>
#include "presentation/ui/MenuController.h"

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
