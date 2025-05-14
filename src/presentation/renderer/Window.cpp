#include <iostream>
#include "presentation/renderer/Window.h"
#include "GLFW/glfw3.h"

Window::Window(GLFWwindow* window) : _window(window) {

}

Window *Window::create(int32_t width, int32_t height) {
    if(!glfwInit()){
        std::cout << "[ERROR] Could not initialize GLFW\n";
        return nullptr;
    }

    GLFWwindow* glfwWindow = glfwCreateWindow(width, height, "Gomoku", nullptr, nullptr);
    if(!glfwWindow){
        glfwTerminate();
        std::cout << "[ERROR] Could not create window\n";
        return nullptr;
    }

    auto* window = new Window(glfwWindow);

    return window;
}

Window::~Window() {
    glfwTerminate();
}

bool Window::shouldClose() {
    return glfwWindowShouldClose(_window);
}
