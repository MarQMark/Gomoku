#ifndef GOMOKU_RENDERER_H
#define GOMOKU_RENDERER_H

#include "glm/glm.hpp"
#include "Window.h"

class Renderer {
public:
    Renderer();
    ~Renderer();

    bool shouldRun();
    void render();

    void drawQuad(glm::vec2 pos, glm::vec2 dim);

private:
    Window* _window{};
};


#endif //GOMOKU_RENDERER_H
