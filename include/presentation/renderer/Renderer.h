#ifndef GOMOKU_RENDERER_H
#define GOMOKU_RENDERER_H

#include <vector>
#include "Shader.h"
#include "glm/glm.hpp"
#include "Window.h"
#include "Batch.h"
#include "Font.h"

class Renderer {
public:
    Renderer();
    ~Renderer();

    bool shouldRun();
    void render();

    /* void drawQuad()
     *  pos:  position of the first vertex (top left corner)
     *
     *  draws quad with following vertex order:
     *      0 -- 1
     *      |    |
     *      3 -- 2
     *
     */
    void drawQuad(glm::vec2 pos, glm::vec2 dim, glm::vec4 color = glm::vec4(1), int32_t layer = 0, uint16_t shader = 0);
    void drawQuadID(uint64_t id, glm::vec2 pos, glm::vec2 dim, glm::vec4 color = glm::vec4(1), int32_t layer = 0, uint16_t shader = 0);

    /* void drawTexture
     *  draw texture2D
     */
    void drawTexture(glm::vec2 pos, glm::vec2 dim, uint16_t txtId, int32_t layer = 0, uint16_t shader = 0);
    void drawTextureID(uint64_t id, glm::vec2 pos, glm::vec2 dim, uint16_t txtId, int32_t layer = 0, uint16_t shader = 0);

    void drawText(std::string text, glm::vec2 pos, float height, float layer = 0, uint16_t shader = 0, Font::Options options = {});
    void drawTextID(uint64_t id, std::string text, glm::vec2 pos, float height, float layer = 0, uint16_t shader = 0, Font::Options options = {});

    /* uint32_t addShader(Shader*)
     *  returns: shader id
     *  parameters:
     *      - pointer to shader
     *
     *   Shaders added to renderer are automatically
     *   freed at end of Renderer lifetime
     */
    uint16_t addShader(Shader* shader);
    uint16_t addTexture(Texture2D* texture);

    glm::vec2 getViewportSize();
    glm::vec2 getCursorPos();

    void setEnforceLayer(bool enforceLayer);

private:
    Window* _window{};

    std::map<uint64_t, Batch*> _batches;
    std::vector<Shader*> _shaders{};
    std::vector<Texture2D*> _textures{};
    std::map<std::string, Font*> _fonts;

    bool _enforce_layer = false;

    Shader* get_shader(uint16_t id);
    Texture2D* get_texture(uint16_t id);
    float map_layer(int32_t layer);
    uint64_t get_batch_id(int32_t layer, uint16_t texture, uint16_t shader);

    void query_errors(const std::string& tag);
};


#endif //GOMOKU_RENDERER_H
