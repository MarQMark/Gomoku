#ifndef GOMOKU_GAME_H
#define GOMOKU_GAME_H


#include "Renderer.h"
#include "presentation/ui/UI.h"
#include "presentation/ui/MenuController.h"
#include "persistence/FileManager.h"
#include "logic/GameService.h"

#ifdef DEBUG
#include "presentation/ui/components/Label.h"
#endif

class Game {
public:
    Game();
    ~Game();

    bool shouldRun();
    void update();

private:
    Renderer* _renderer{};
    UI* _ui{};
    MenuController* _menu_controller{};
    FileManager* _file_manager{};
    GameService* _game_service{};
#ifdef DEBUG
    Label* _fps_lbl{};
#endif
};


#endif //GOMOKU_GAME_H
