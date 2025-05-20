#include <iostream>
#include "presentation/renderer/Window.h"
#include "GLFW/glfw3.h"

Window::Window(int32_t width, int32_t height){
    if(!glfwInit()){
        throw std::runtime_error("Could not initialize GLFW\n");
    }

    _window = glfwCreateWindow(width, height, "Gomoku", nullptr, nullptr);
    if(!_window){
        glfwTerminate();
        throw std::runtime_error("Could not create window\n");
    }

    glfwMakeContextCurrent(_window);
}

Window::~Window() {
    glfwTerminate();
}

bool Window::shouldClose() {
    return glfwWindowShouldClose(_window);
}

GLFWwindow *Window::getGLFWWindow() const{
    return _window;
}
