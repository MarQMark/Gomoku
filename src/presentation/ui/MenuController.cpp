#include <iostream>
#include "presentation/ui/MenuController.h"

void MenuController::update() {
    if(!_menu_views[_last_menu]->inMotion())
        _menu_views[_curr_menu]->setHidden(false);
}

void MenuController::addMenuView(Menu menu, MenuView *menuView) {
    if(_menu_views.count(menu)){
        std::cout << "[Warning] MenuView " << menu << " already exists in Controller\n";
        return;
    }

    _menu_views[menu] = menuView;
}

void MenuController::changeMenu(Menu menu) {
    _last_menu = _curr_menu;
    _menu_views[_last_menu]->setHidden(true);
    _curr_menu = menu;
}
