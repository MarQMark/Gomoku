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
    void drawQuad(glm::vec2 pos, glm::vec2 dim, glm::vec4 color = glm::vec4(1), int32_t layer = 0, std::string shader = "default");
    void drawQuadID(uint64_t id, glm::vec2 pos, glm::vec2 dim, glm::vec4 color = glm::vec4(1), int32_t layer = 0, std::string shader = "default");

    /* void drawTexture
     *  draw texture2D
     */
    void drawTexture(glm::vec2 pos, glm::vec2 dim, Texture2D* texture, int32_t layer = 0, std::string shader = "default");
    void drawTextureID(uint64_t id, glm::vec2 pos, glm::vec2 dim, Texture2D* texture, int32_t layer = 0, std::string shader = "default");

    void drawText(std::string text, glm::vec2 pos, float height, float layer = 0, std::string shader = "default", Font::Options options = {});
    void drawTextID(uint64_t id, std::string text, glm::vec2 pos, float height, float layer = 0, std::string shader = "default", Font::Options options = {});

    /* uint32_t addShader(Shader*)
     *  returns: shader id
     *  parameters:
     *      - pointer to shader
     *
     *   Shaders added to renderer are automatically
     *   freed at end of Renderer lifetime
     */
    int addShader(Shader* shader, std::string name);
    int addTexture(Texture2D* texture, std::string name);
    Texture2D* getTexture(std::string name);
    Font* getFont(std::string name);

    glm::vec2 getViewportSize();
    glm::vec2 getCursorPos();

    void setEnforceLayer(bool enforceLayer);

private:
    Window* _window{};

    std::map<uint64_t, Batch*> _batches;
    std::map<std::string, Shader*> _shaders{};
    std::map<std::string, Texture2D*> _textures{};
    std::map<std::string, Font*> _fonts;

    bool _enforce_layer = false;

    Shader* get_shader(std::string name = "default");
    float map_layer(int32_t layer);
    uint64_t get_batch_id(int32_t layer, uint16_t texture, std::string shader);

    void query_errors(const std::string& tag);
};


#endif //GOMOKU_RENDERER_H
