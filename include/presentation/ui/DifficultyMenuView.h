#ifndef GOMOKU_DIFFICULTYMENUVIEW_H
#define GOMOKU_DIFFICULTYMENUVIEW_H


#include "presentation/ui/components/MenuView.h"
#include "MenuController.h"

class DifficultyMenuView : public MenuView {
public:
    DifficultyMenuView(std::string name, MenuController* menuController);
    ~DifficultyMenuView() override;

private:

};


#endif //GOMOKU_DIFFICULTYMENUVIEW_H
