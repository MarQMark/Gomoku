#ifndef GOMOKU_WINDOW_H
#define GOMOKU_WINDOW_H


#include <cstdint>
#include "GLFW/glfw3.h"

class Window {
public:
    static Window* create(int32_t width, int32_t height);
    ~Window();

    bool shouldClose();

private:
    explicit Window(GLFWwindow* window);

    GLFWwindow* _window {};
};


#endif //GOMOKU_WINDOW_H
