#include "presentation/renderer/Renderer.h"

Renderer::Renderer() {
    _window = new Window(1280, 720);
}

Renderer::~Renderer() {
    delete _window;
}

void Renderer::drawQuad(glm::vec2 pos, glm::vec2 dim) {

}

bool Renderer::shouldRun() {
    return !_window->shouldClose();
}

void Renderer::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glfwSwapBuffers(_window->getGLFWWindow());
    glfwPollEvents();
}
