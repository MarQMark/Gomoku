#ifndef GOMOKU_RENDERER_H
#define GOMOKU_RENDERER_H

#include <vector>
#include "Shader.h"
#include "glm/glm.hpp"
#include "Window.h"
#include "Batch.h"

class Renderer {
public:
    Renderer();
    ~Renderer();

    bool shouldRun();
    void render();

    void drawQuad(std::string id, glm::vec2 pos, glm::vec2 dim, int32_t layer = 0, uint16_t shader = UINT16_MAX);

    /* uint32_t addShader(Shader*)
     *  returns: shader id
     *  parameters:
     *      - pointer to shader
     *
     *   Shaders added to renderer are automatically
     *   freed at end of Renderer lifetime
     */
    uint16_t addShader(Shader* shader);

private:
    Window* _window{};

    std::map<uint64_t, Batch*> _batches;
    std::vector<Shader*> _shaders{};

    Shader* get_shader(uint16_t id);
    uint64_t get_batch_id(int32_t layer, uint16_t texture, uint16_t shader);
};


#endif //GOMOKU_RENDERER_H
