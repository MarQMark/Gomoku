#ifndef GOMOKU_MAINMENUVIEW_H
#define GOMOKU_MAINMENUVIEW_H

#include "presentation/ui/components/MenuView.h"
#include "MenuController.h"

class MainMenuView : public MenuView {
public:
    MainMenuView(std::string name, MenuController* menuController);
    ~MainMenuView() override;

private:

};


#endif //GOMOKU_MAINMENUVIEW_H
