#include <iostream>
#include <functional>
#include <cstring>
#include <sstream>
#include "presentation/renderer/Renderer.h"
#include "presentation/renderer/buffer/Vertex.h"
#include "presentation/input/Input.h"
#include "presentation/renderer/font/KikanFont.h"
#include "presentation/renderer/font/GomokuFont.h"

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

    addShader(new Shader("shaders/default.vert", "shaders/default.frag"), "default");
    addShader(new Shader("shaders/default.vert", "shaders/grid.frag"), "grid");
    addShader(new Shader("shaders/default.vert", "shaders/font.frag"), "font");
    addShader(new Shader("shaders/stone.vert", "shaders/default.frag"), "stone");
    addShader(new Shader("shaders/board.vert", "shaders/default.frag"), "board");
    // TODO: FIX
    addShader(new Shader("shaders/default.vert", "shaders/bg.frag"), "bg");
    get_shader("bg")->uniform2fv("u_resolution", glm::vec2(1280, 720));

    _fonts["default"] = new Font((void*)gomokuFont);
    _fonts["kikan"] = new Font((void*)kikanFont);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    query_errors("Renderer::Constructor");
}

Renderer::~Renderer() {
    for (const auto& pair : _shaders)
        delete pair.second;

    for (const auto& pair : _textures)
        delete pair.second;

    for (auto pair : _batches)
        delete pair.second;

    delete _window;
}

BATCH_FUNC void Renderer::drawQuad(glm::vec2 pos, glm::vec2 dim, glm::vec4 color, const Options& options) {
    BATCH_FORCE_STACK
    uint64_t buffId = GET_RED_ADDR;

    drawQuadID(buffId, pos, dim, color, options);
}

BATCH_FUNC void Renderer::drawTexture(glm::vec2 pos, glm::vec2 dim, Texture2D* texture, const Options& options) {
    BATCH_FORCE_STACK
    uint64_t buffId = GET_RED_ADDR;

    drawTextureID(buffId, pos, dim, texture, options);
}


BATCH_FUNC void Renderer::drawText(std::string text, glm::vec2 pos, float height, const Options& options, Font::Options fontOptions) {
    BATCH_FORCE_STACK
    uint64_t buffId = GET_RED_ADDR;

    drawTextID(buffId, text, pos, height, options, fontOptions);
}

void Renderer::drawQuadID(uint64_t id, glm::vec2 pos, glm::vec2 dim, glm::vec4 color, const Options& options){
    BatchID batchID;
    batchID.texture = -1;
    batchID.shader = std::hash<std::string>{}(options.shaderName);
    batchID.animation = options.animationID;
    if(_enforce_layer)
        batchID.layer = options.layer;

    if(!_batches.count(batchID))
        _batches[batchID] = new Batch(nullptr, get_shader(options.shaderName));

    _batches[batchID]->setAnimator(options.animator);

    // Create vertices
    Vertex* vertices = (Vertex*) malloc(sizeof(Vertex) * 4);
    vertices[0].position = glm::vec3(pos,                           map_layer(options.layer));
    vertices[1].position = glm::vec3(pos + glm::vec2(dim.x, 0),     map_layer(options.layer));
    vertices[2].position = glm::vec3(pos + glm::vec2(dim.x, dim.y), map_layer(options.layer));
    vertices[3].position = glm::vec3(pos + glm::vec2(0,     dim.y), map_layer(options.layer));
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

void Renderer::drawTextureID(uint64_t id, glm::vec2 pos, glm::vec2 dim, Texture2D* texture, const Options& options){
    BatchID batchID;
    batchID.texture = texture->get();
    batchID.shader = std::hash<std::string>{}(options.shaderName);
    batchID.animation = options.animationID;
    if(_enforce_layer)
        batchID.layer = options.layer;

    if(!_batches.count(batchID))
        _batches[batchID] = new Batch(texture, get_shader(options.shaderName));

    _batches[batchID]->setAnimator(options.animator);

    // Create vertices
    Vertex* vertices = (Vertex*) malloc(sizeof(Vertex) * 4);
    vertices[0].position = glm::vec3(pos,                           map_layer(options.layer));
    vertices[1].position = glm::vec3(pos + glm::vec2(dim.x, 0),     map_layer(options.layer));
    vertices[2].position = glm::vec3(pos + glm::vec2(dim.x, dim.y), map_layer(options.layer));
    vertices[3].position = glm::vec3(pos + glm::vec2(0,     dim.y), map_layer(options.layer));
    vertices[0].texCoords = glm::vec2(0, 0);
    vertices[1].texCoords = glm::vec2(1, 0);
    vertices[2].texCoords = glm::vec2(1, 1);
    vertices[3].texCoords = glm::vec2(0, 1);
    glm::vec2 center = pos + (dim / 2.f);
    for(int i = 0; i < 4; i++){
        vertices[i].color = glm::vec4(0);
        vertices[i].normal = glm::normalize(glm::vec2(vertices[i].position) - center);
    }

    // Create indices
    uint32_t* indices = (uint32_t*) malloc(sizeof(uint32_t) * 6);
    uint32_t temp[] = {0, 1, 2, 0, 2, 3};
    memcpy(indices, temp, sizeof(temp));

    _batches[batchID]->updateBuffer(id, vertices, sizeof(Vertex) * 4, indices, 6);
}

void Renderer::drawTextID(uint64_t id, const std::string& text, glm::vec2 pos, float height, const Options& options, Font::Options fontOptions){
    Font* font = fontOptions.font;
    if(!font)
        font = _fonts["default"];

    BatchID batchID;
    batchID.texture = font->getID();
    batchID.shader = std::hash<std::string>{}(options.shaderName);
    batchID.animation = options.animationID;
    if(_enforce_layer)
        batchID.layer = options.layer;

    if(!_batches.count(batchID))
        _batches[batchID] = new Batch(font->getTexture(), get_shader(options.shaderName));

    _batches[batchID]->setAnimator(options.animator);

    Font::Glyph* g = font->getGlyph('A');
    glm::vec2 viewportDim = getViewportSize();
    const float scale = (height / g->dim.y);
    const float whitespace = scale * g->dim.x * (viewportDim.y / viewportDim.x);

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
            y += height * 1.5f * fontOptions.spacing.y;
            continue;
        }

        g = font->getGlyph(c);
        float cWidth = g->dim.x * scale * (viewportDim.y / viewportDim.x);
        float cHeight = g->dim.y * scale;
        float offX = g->offset.x * scale;
        float offY = g->offset.y * scale;

        //Position
        vertices[nVertex + 0].position = glm::vec3(x + offX,          y + offY,           map_layer(options.layer));
        vertices[nVertex + 1].position = glm::vec3(x + offX + cWidth, y + offY,           map_layer(options.layer));
        vertices[nVertex + 2].position = glm::vec3(x + offX + cWidth, y + offY + cHeight, map_layer(options.layer));
        vertices[nVertex + 3].position = glm::vec3(x + offX,          y + offY + cHeight, map_layer(options.layer));

        // set Texture Coords
        vertices[nVertex + 0].texCoords = glm::vec2(g->pos.x,               1 - (g->pos.y));
        vertices[nVertex + 1].texCoords = glm::vec2(g->pos.x + g->dim.x,    1 - (g->pos.y));
        vertices[nVertex + 2].texCoords = glm::vec2(g->pos.x + g->dim.x,    1 - (g->pos.y + g->dim.y));
        vertices[nVertex + 3].texCoords = glm::vec2(g->pos.x,               1 - (g->pos.y + g->dim.y));

        for (int i = 0; i < 4; ++i)
            vertices[nVertex + i].color = fontOptions.color;

        indices[indexCnt++] = nVertex + 0;
        indices[indexCnt++] = nVertex + 1;
        indices[indexCnt++] = nVertex + 2;
        indices[indexCnt++] = nVertex + 0;
        indices[indexCnt++] = nVertex + 2;
        indices[indexCnt++] = nVertex + 3;

        nVertex+=4;

        x += cWidth + whitespace * fontOptions.spacing.x;
    }

    _batches[batchID]->updateBuffer(id, vertices, sizeof(Vertex) * 4 * textLen, indices, 6 * textLen);
}

bool Renderer::shouldRun() {
    return !_window->shouldClose();
}

void Renderer::render() {
    glClearColor(0.0, 0.0, 0.0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    query_errors("Renderer::render");

    std::vector<BatchID> emptyBatches;
    for (auto pair : _batches) {
        if(pair.second->render() == -1)
            emptyBatches.push_back(pair.first);
    }
    for(auto id : emptyBatches){
        delete _batches[id];
        _batches.erase(id);
    }

    _window->update();
    Input::get()->update();
}

int Renderer::addShader(Shader *shader, const std::string& name) {
    if(_shaders.count(name)){
        std::cout << "[WARNING] Shader with same name already exists\n";
        return -1;
    }

    _shaders[name] = shader;
    return 1;
}

uint64_t Renderer::get_batch_id(float layer, uint16_t texture, const std::string& shader) {
    uint16_t shaderHash = std::hash<std::string>{}(shader);
    return (uint64_t) layer << 32 | texture << 16 | shaderHash;
}

Shader *Renderer::get_shader(const std::string& name) {
    if(!_shaders.count(name))
        return nullptr;

    return _shaders[name];
}

void Renderer::setEnforceLayer(bool enforceLayer) {
    _enforce_layer = enforceLayer;
}

float Renderer::map_layer(float layer) {
    return 1 / (1 + std::exp(layer));
}

int Renderer::addTexture(Texture2D *texture, const std::string& name) {
    if(_textures.count(name)){
        std::cout << "[WARNING] Texture with same name already exists\n";
        return -1;
    }

    _textures[name] = texture;
    return 1;
}

Texture2D *Renderer::getTexture(const std::string& name) {
    if(!_textures.count(name))
        return nullptr;

    return _textures[name];
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

Font *Renderer::getFont(const std::string& name) {
    if(!_fonts.count(name))
        return nullptr;

    return _fonts[name];
}
