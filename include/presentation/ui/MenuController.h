#ifndef GOMOKU_MENUCONTROLLER_H
#define GOMOKU_MENUCONTROLLER_H


#include "logic/DTO/CommandDTOs.h"
#include "logic/interfaces/IMenuEventListener.h"
#include "presentation/ui/components/MenuView.h"

class IGameService;

class MenuController : IMenuEventListener {
public:
    enum Menu{
        Main,
        Mode,
        Difficulty,
        Game
    };

    ~MenuController() = default;
    explicit MenuController(IGameService *gameService);

    void update();

    void addMenuView(Menu, MenuView* menuView);
    void changeMenu(Menu);

    IGameService *getGameService() const;
    GameMode getSelectedGameMode() const;
    void setSelectedGameMode(GameMode selected_game_mode);

    void onGameCompleted() override;
    void onStatsChanged(StatsViewDTO stats) override;

private:
    IGameService* _game_service;
    std::map<Menu, std::vector<MenuView*>> _menu_views;
    GameMode _selected_game_mode;
    Menu _curr_menu = Main;
    Menu _last_menu = Main;
};


#endif //GOMOKU_MENUCONTROLLER_H
