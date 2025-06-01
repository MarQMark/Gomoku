#ifndef GOMOKU_MENUCONTROLLER_H
#define GOMOKU_MENUCONTROLLER_H


#include "presentation/ui/components/MenuView.h"

class MenuController {
public:
    enum Menu{
        Main,
        Mode,
        Difficulty,
        Game
    };

    MenuController() = default;
    ~MenuController() = default;

    void update();

    void addMenuView(Menu, MenuView* menuView);

    void changeMenu(Menu);

private:
    std::map<Menu, MenuView*> _menu_views;
    Menu _curr_menu = Main;
    Menu _last_menu = Main;
};


#endif //GOMOKU_MENUCONTROLLER_H
