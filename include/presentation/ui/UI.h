#ifndef GOMOKU_UI_H
#define GOMOKU_UI_H

#include "Renderer.h"
#include "View.h"

class UI {
public:
    UI(Renderer* renderer);

    void update();

    View* _view_root;
private:
    Renderer* _renderer{};

};


#endif //GOMOKU_UI_H
