#ifndef GOMOKU_GAMEMENUVIEW_H
#define GOMOKU_GAMEMENUVIEW_H

#include "presentation/ui/components/MenuView.h"
#include "presentation/ui/MenuController.h"
#include "presentation/ui/components/DrawerView.h"

class GameMenuView : public MenuView {
public:
    GameMenuView(std::string name, MenuController* menuController);
    ~GameMenuView();

    void setHidden(bool hidden) override;

private:
    DrawerView* _drawer_view{};
};


#endif //GOMOKU_GAMEMENUVIEW_H
