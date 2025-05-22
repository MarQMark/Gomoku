#include <iostream>
#include <functional>
#include <cstring>
#include "presentation/renderer/Renderer.h"
#include "Vertex.h"
#include "Shaders.h"

#if defined(__MSC_VER)
#include <intrin.h>
#pragma intrinsic(_ReturnAddress)

#define BATCH_FUNC __declspec(noinline)
#define GET_RED_ADDR __ReturnAddress()

#elif defined(__GNUC__) || defined(__clang__)
#define BATCH_FUNC __attribute__((noinline))
#define GET_RED_ADDR (uint64_t)__builtin_return_address(0)
#endif

#define BATCH_FORCE_STACK volatile int force_stack = 1;

#if !defined(__clang__) && !defined(__GNUC__) && !defined(__MSC_VER)
#error "Incompatible Compiler"
#endif



Renderer::Renderer() {
    _window = new Window(1280, 720);

    if (glewInit() != GLEW_OK)
        throw std::runtime_error("Error initializing glew\n");

    Shader* shader = new Shader("shaders/default.vert", "shaders/default.frag");
    addShader(shader);
    shader = new Shader("shaders/default.vert", "shaders/grid.frag");
    addShader(shader);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
}

Renderer::~Renderer() {
    for (auto* shader : _shaders)
        delete shader;

    for (auto pair : _batches)
        delete pair.second;

    delete _window;
}

BATCH_FUNC void Renderer::drawQuad(glm::vec2 pos, glm::vec2 dim, glm::vec4 color, int32_t layer, uint16_t shader) {
    BATCH_FORCE_STACK
    uint64_t buffId = GET_RED_ADDR;

    uint64_t batchID = get_batch_id(0, -1, shader);
    if(_enforce_layer)
        batchID = get_batch_id(layer, -1, shader);

    if(!_batches.count(batchID))
        _batches[batchID] = new Batch(nullptr, get_shader(shader));

    // Create vertices
    Vertex* vertices = (Vertex*) malloc(sizeof(Vertex) * 4);
    vertices[0].position = glm::vec3(pos,                            map_layer(layer));
    vertices[1].position = glm::vec3(pos + glm::vec2(dim.x, 0),      map_layer(layer));
    vertices[2].position = glm::vec3(pos + glm::vec2(dim.x, -dim.y), map_layer(layer));
    vertices[3].position = glm::vec3(pos + glm::vec2(0,     -dim.y), map_layer(layer));
    for(int i = 0; i < 4; i++){
        vertices[i].color = color;
        vertices[i].texCoords = glm::vec2(0);
    }

    // Create indices
    uint32_t* indices = (uint32_t*) malloc(sizeof(uint32_t) * 6);
    uint32_t temp[] = {0, 1, 2, 0, 2, 3};
    memcpy(indices, temp, sizeof(temp));

    _batches[batchID]->updateBuffer(buffId, vertices, sizeof(Vertex) * 4, indices, 6);
}

BATCH_FUNC void Renderer::drawTexture(glm::vec2 pos, glm::vec2 dim, uint16_t txtId, int32_t layer, uint16_t shader) {
    BATCH_FORCE_STACK
    uint64_t buffId = GET_RED_ADDR;

    uint64_t batchID = get_batch_id(0, txtId, shader);
    if(_enforce_layer)
        batchID = get_batch_id(layer, txtId, shader);

    if(!_batches.count(batchID))
        _batches[batchID] = new Batch(get_texture(txtId), get_shader(shader));

    // Create vertices
    Vertex* vertices = (Vertex*) malloc(sizeof(Vertex) * 4);
    vertices[0].position = glm::vec3(pos,                            map_layer(layer));
    vertices[1].position = glm::vec3(pos + glm::vec2(dim.x, 0),      map_layer(layer));
    vertices[2].position = glm::vec3(pos + glm::vec2(dim.x, -dim.y), map_layer(layer));
    vertices[3].position = glm::vec3(pos + glm::vec2(0,     -dim.y), map_layer(layer));
    vertices[0].texCoords = glm::vec2(1, 0);
    vertices[1].texCoords = glm::vec2(0, 0);
    vertices[2].texCoords = glm::vec2(0, 1);
    vertices[3].texCoords = glm::vec2(1, 1);
    for(int i = 0; i < 4; i++){
        vertices[i].color = glm::vec4(0);
    }

    // Create indices
    uint32_t* indices = (uint32_t*) malloc(sizeof(uint32_t) * 6);
    uint32_t temp[] = {0, 1, 2, 0, 2, 3};
    memcpy(indices, temp, sizeof(temp));

    _batches[batchID]->updateBuffer(buffId, vertices, sizeof(Vertex) * 4, indices, 6);
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

void Renderer::setEnforceLayer(bool enforceLayer) {
    _enforce_layer = enforceLayer;
}

float Renderer::map_layer(int32_t layer) {
    return 1 / (1 + std::exp((float)layer));
}

uint16_t Renderer::addTexture(Texture2D *texture) {
    _textures.push_back(texture);
    return _textures.size() - 1;
}

Texture2D *Renderer::get_texture(uint16_t id) {
    if(id >= _textures.size())
        return nullptr;

    return _textures[id];
}
