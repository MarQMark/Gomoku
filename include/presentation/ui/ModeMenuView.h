#ifndef GOMOKU_MODEMENUVIEW_H
#define GOMOKU_MODEMENUVIEW_H


#include "presentation/ui/components/MenuView.h"
#include "MenuController.h"

class ModeMenuView : public MenuView {
public:
    ModeMenuView(std::string name, MenuController* menuController);
    ~ModeMenuView() override;

private:
};


#endif //GOMOKU_MODEMENUVIEW_H
