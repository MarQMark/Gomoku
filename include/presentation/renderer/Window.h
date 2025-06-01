#ifndef GOMOKU_WINDOW_H
#define GOMOKU_WINDOW_H


#include <cstdint>
#include "GLFW/glfw3.h"

class Window {
public:
    explicit Window(int32_t width, int32_t height);
    ~Window();

    GLFWwindow* getGLFWWindow() const;
    bool shouldClose();

    void getSize(int* width, int* height);
    void getCursorPos(double* x, double* y);

    void setIcon(int width, int height, unsigned char* data);

    void update();

private:
    GLFWwindow* _window {};
    double _icon_cooldown = 0;
};


#endif //GOMOKU_WINDOW_H
