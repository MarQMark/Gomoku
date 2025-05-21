#ifndef GOMOKU_UI_H
#define GOMOKU_UI_H

#include "Renderer.h"

class UI {
public:
    UI(Renderer* renderer);

    void update();

private:
    Renderer* _renderer{};

    //std::vector
};


#endif //GOMOKU_UI_H
