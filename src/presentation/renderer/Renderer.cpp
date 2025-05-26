#include <iostream>
#include <functional>
#include <cstring>
#include <sstream>
#include "presentation/renderer/Renderer.h"
#include "Vertex.h"
#include "Shaders.h"
#include "presentation/input/Input.h"
#include "presentation/renderer/DefaultFont.h"

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

    addShader(new Shader("shaders/default.vert", "shaders/default.frag"));
    addShader(new Shader("shaders/default.vert", "shaders/grid.frag"));
    addShader(new Shader("shaders/default.vert", "shaders/font.frag"));

    _fonts["default"] = new Font((void*)defaultFont);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    query_errors("Renderer::Constructor");
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

    drawQuadID(buffId, pos, dim, color, layer, shader);
}

BATCH_FUNC void Renderer::drawTexture(glm::vec2 pos, glm::vec2 dim, uint16_t txtId, int32_t layer, uint16_t shader) {
    BATCH_FORCE_STACK
    uint64_t buffId = GET_RED_ADDR;

    drawTextureID(buffId, pos, dim, txtId, layer, shader);
}


BATCH_FUNC void Renderer::drawText(std::string text, glm::vec2 pos, float height, float layer, uint16_t shader, Font::Options options) {
    BATCH_FORCE_STACK
    uint64_t buffId = GET_RED_ADDR;

    drawTextID(buffId, text, pos, height, layer, shader, options);
}

void Renderer::drawQuadID(uint64_t id, glm::vec2 pos, glm::vec2 dim, glm::vec4 color, int32_t layer, uint16_t shader){
    uint64_t batchID = get_batch_id(0, -1, shader);
    if(_enforce_layer)
        batchID = get_batch_id(layer, -1, shader);

    if(!_batches.count(batchID))
        _batches[batchID] = new Batch(nullptr, get_shader(shader));

    // Create vertices
    Vertex* vertices = (Vertex*) malloc(sizeof(Vertex) * 4);
    vertices[0].position = glm::vec3(pos,                            map_layer(layer));
    vertices[1].position = glm::vec3(pos + glm::vec2(dim.x, 0),      map_layer(layer));
    vertices[2].position = glm::vec3(pos + glm::vec2(dim.x, dim.y), map_layer(layer));
    vertices[3].position = glm::vec3(pos + glm::vec2(0,     dim.y), map_layer(layer));
    for(int i = 0; i < 4; i++){
        vertices[i].color = color;
        vertices[i].texCoords = glm::vec2(0);
    }

    // Create indices
    uint32_t* indices = (uint32_t*) malloc(sizeof(uint32_t) * 6);
    uint32_t temp[] = {0, 1, 2, 0, 2, 3};
    memcpy(indices, temp, sizeof(temp));

    _batches[batchID]->updateBuffer(id, vertices, sizeof(Vertex) * 4, indices, 6);
}

void Renderer::drawTextureID(uint64_t id, glm::vec2 pos, glm::vec2 dim, uint16_t txtId, int32_t layer, uint16_t shader){
    uint64_t batchID = get_batch_id(0, txtId, shader);
    if(_enforce_layer)
        batchID = get_batch_id(layer, txtId, shader);

    if(!_batches.count(batchID))
        _batches[batchID] = new Batch(get_texture(txtId), get_shader(shader));

    // Create vertices
    Vertex* vertices = (Vertex*) malloc(sizeof(Vertex) * 4);
    vertices[0].position = glm::vec3(pos,                           map_layer(layer));
    vertices[1].position = glm::vec3(pos + glm::vec2(dim.x, 0),     map_layer(layer));
    vertices[2].position = glm::vec3(pos + glm::vec2(dim.x, dim.y), map_layer(layer));
    vertices[3].position = glm::vec3(pos + glm::vec2(0,     dim.y), map_layer(layer));
    vertices[0].texCoords = glm::vec2(0, 0);
    vertices[1].texCoords = glm::vec2(1, 0);
    vertices[2].texCoords = glm::vec2(1, 1);
    vertices[3].texCoords = glm::vec2(0, 1);
    for(int i = 0; i < 4; i++){
        vertices[i].color = glm::vec4(0);
    }

    // Create indices
    uint32_t* indices = (uint32_t*) malloc(sizeof(uint32_t) * 6);
    uint32_t temp[] = {0, 1, 2, 0, 2, 3};
    memcpy(indices, temp, sizeof(temp));

    _batches[batchID]->updateBuffer(id, vertices, sizeof(Vertex) * 4, indices, 6);
}

void Renderer::drawTextID(uint64_t id, std::string text, glm::vec2 pos, float height, float layer, uint16_t shader, Font::Options options){
    Font* font = options.font;
    if(!font)
        font = _fonts["default"];

    uint64_t batchID = get_batch_id(0, font->getID(), shader);
    if(_enforce_layer)
        batchID = get_batch_id(layer, font->getID(), shader);

    if(!_batches.count(batchID))
        _batches[batchID] = new Batch(font->getTexture(), get_shader(shader));

    Font::Glyph* g = font->getGlyph('A');
    const float scale = height/g->dim.y;
    const float whitespace = scale * g->dim.x;

    uint32_t textLen = 0;
    for (char c : text) {
        if(c != ' ' && c != '\t' && c != '\r' && c != '\n')
            textLen++;
    }

    Vertex* vertices = (Vertex*)malloc(sizeof(Vertex) * 4 * textLen);
    uint32_t* indices = (uint32_t*)malloc(sizeof(uint32_t) * 6 * textLen);

    uint32_t indexCnt = 0;
    uint32_t nVertex = 0;
    float y = pos.y;
    float x = pos.x;

    for (char c : text) {
        if(c == ' ')       { x += whitespace;      continue; } // Whitespace
        else if(c == '\t') { x += whitespace * 4;  continue; } // Tab
        else if(c == '\r') { x = pos.x;            continue; } // Carriage Return
        else if(c == '\n') {                                   // Newline
            x = pos.x;
            y-= height * 1.5f * options.spacing.y;
            continue;
        }

        g = font->getGlyph(c);
        float cWidth = g->dim.x * scale;
        float cHeight = g->dim.y * scale;
        float offX = g->offset.x * scale;
        float offY = g->offset.y * scale;

        //Position
        vertices[nVertex + 0].position = glm::vec3(x + offX,          y + offY,           map_layer(layer));
        vertices[nVertex + 1].position = glm::vec3(x + offX + cWidth, y + offY,           map_layer(layer));
        vertices[nVertex + 2].position = glm::vec3(x + offX + cWidth, y + offY + cHeight, map_layer(layer));
        vertices[nVertex + 3].position = glm::vec3(x + offX,          y + offY + cHeight, map_layer(layer));

        // set Texture Coords
        vertices[nVertex + 0].texCoords = glm::vec2(g->pos.x,               1 - (g->pos.y));
        vertices[nVertex + 1].texCoords = glm::vec2(g->pos.x + g->dim.x,    1 - (g->pos.y));
        vertices[nVertex + 2].texCoords = glm::vec2(g->pos.x + g->dim.x,    1 - (g->pos.y + g->dim.y));
        vertices[nVertex + 3].texCoords = glm::vec2(g->pos.x,               1 - (g->pos.y + g->dim.y));

        for (int i = 0; i < 4; ++i)
            vertices[nVertex + i].color = options.color;

        indices[indexCnt++] = nVertex + 0;
        indices[indexCnt++] = nVertex + 1;
        indices[indexCnt++] = nVertex + 2;
        indices[indexCnt++] = nVertex + 0;
        indices[indexCnt++] = nVertex + 2;
        indices[indexCnt++] = nVertex + 3;

        nVertex+=4;

        x += cWidth + whitespace * options.spacing.x;
    }

    _batches[batchID]->updateBuffer(id, vertices, sizeof(Vertex) * 4 * textLen, indices, 6 * textLen);
}

bool Renderer::shouldRun() {
    return !_window->shouldClose();
}

void Renderer::render() {
    glClearColor(.6, .2, .8, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    query_errors("Renderer::render");

    for (auto pair : _batches) {
        pair.second->render();
    }

    glfwSwapBuffers(_window->getGLFWWindow());
    glfwPollEvents();

    Input::get()->update();
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

glm::vec2 Renderer::getViewportSize() {
    int width, height;
    _window->getSize(&width, &height);
    return glm::vec2(width, height);
}

glm::vec2 Renderer::getCursorPos() {
    double x, y;
    _window->getCursorPos(&x, &y);
    return glm::vec2(x, y);
}

void Renderer::query_errors(const std::string& tag) {
    GLenum err = glGetError();
    while(err != GL_NO_ERROR){
        std::stringstream ss;
        ss<< std::hex << err; // int decimal_value
        std::string res ( ss.str() );
        std::cout << "[OPENGL ERROR] in " << tag << ": " << res << std::endl;
        err = glGetError();
    }
}
