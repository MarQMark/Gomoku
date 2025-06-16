#include <iostream>
#include "presentation/renderer/Window.h"
#include "GLFW/glfw3.h"
#include "presentation/input/Input.h"
#include "common/Time.h"

void window_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

Window::Window(int32_t width, int32_t height){
    if(!glfwInit()){
        throw std::runtime_error("Could not initialize GLFW\n");
    }

#ifdef DEBUG
    _window = glfwCreateWindow(width, height, "Gomoku Debug", nullptr, nullptr);
#else
    _window = glfwCreateWindow(width, height, "Gomoku", nullptr, nullptr);
#endif
    if(!_window){
        glfwTerminate();
        throw std::runtime_error("Could not create window\n");
    }

    glfwMakeContextCurrent(_window);
    glfwSetWindowSizeCallback(_window, window_size_callback);

    Input::init(_window);
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

void Window::getSize(int* width, int *height) {
    glfwGetWindowSize(_window, width, height);
}

void Window::getCursorPos(double *x, double *y) {
    glfwGetCursorPos(_window, x, y);
}

void Window::update() {
    _icon_cooldown = std::max(_icon_cooldown - Time::get()->getDeltaTime(), 0.);

    glfwSwapBuffers(_window);
    glfwPollEvents();
}

void Window::setIcon(int width, int height, unsigned char *data) {
    if(!data || _icon_cooldown != 0)
        return;

    _icon_cooldown = .05;

    GLFWimage icon;
    icon.width = width;
    icon.height = height;
    icon.pixels = data;
    glfwSetWindowIcon(_window, 1, &icon);
}
