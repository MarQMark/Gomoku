#include <iostream>
#include "presentation/renderer/Renderer.h"

Renderer::Renderer() {
    _window = new Window(1280, 720);
}

Renderer::~Renderer() {
    for (auto* shader : _shaders)
        delete shader;

    for (auto pair : _batches)
        delete pair.second;

    delete _window;
}

void Renderer::drawQuad(std::string id, glm::vec2 pos, glm::vec2 dim, int32_t layer, uint16_t shader) {
    uint64_t batchID = get_batch_id(layer, 0, shader);
    //if(!_batches.count(batchID))
        //_batches[batchID] = new Batch(nullptr, get_shader(shader));

    /* TODO:
     *   - Batch add vertices
     *   - Init Vertex
     *   - How to update Vertex Buffer/ Index Buffer
     */
    std::cout << "id: " << id << "\n";
}

bool Renderer::shouldRun() {
    return !_window->shouldClose();
}

void Renderer::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (auto pair : _batches) {
        pair.second->render();
    }

    glfwSwapBuffers(_window->getGLFWWindow());
    glfwPollEvents();
}

uint16_t Renderer::addShader(Shader *shader) {
    // return UINT16_MAX if not enough ids for shaders
    if(_shaders.size() == UINT16_MAX - 1){
        std::cout << "[ERROR] Shader limit reached\n";
        return UINT16_MAX;
    }

    _shaders.push_back(shader);
    return _shaders.size() - 1;
}

uint64_t Renderer::get_batch_id(int32_t layer, uint16_t texture, uint16_t shader) {
    return (uint64_t) layer << 32 | texture << 16 | shader;
}

Shader *Renderer::get_shader(uint16_t id) {
    if(id >= _shaders.size())
        return nullptr;

    return _shaders[id];
}
