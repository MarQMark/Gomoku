#ifndef GOMOKU_RENDERER_H
#define GOMOKU_RENDERER_H

#include <vector>
#include "Shader.h"
#include "glm/glm.hpp"
#include "Window.h"
#include "Batch.h"
#include "presentation/renderer/font/Font.h"
#include "presentation/renderer/animator/Animator.h"

class Renderer {
public:
    Renderer();
    ~Renderer();

    bool shouldRun();
    void render();

    struct Options{
        Options() {};
        float layer = 0;
        std::string shaderName = "default";
        uint64_t animationID = 0;
        Animator* animator{};
    };

    /* void drawQuad()
     *  pos:  position of the first vertex (top left corner)
     *
     *  draws quad with following vertex order:
     *      0 -- 1
     *      |    |
     *      3 -- 2
     *
     */
    void drawQuad(glm::vec2 pos, glm::vec2 dim, glm::vec4 color = glm::vec4(1), const Options& options = {});
    void drawQuadID(uint64_t id, glm::vec2 pos, glm::vec2 dim, glm::vec4 color = glm::vec4(1), const Options& options = {});

    /* void drawTexture
     *  draw texture2D
     */
    void drawTexture(glm::vec2 pos, glm::vec2 dim, Texture2D* texture, const Options& options = {});
    void drawTextureID(uint64_t id, glm::vec2 pos, glm::vec2 dim, Texture2D* texture, const Options& options = {});

    void drawText(std::string text, glm::vec2 pos, float height, const Options& options = {}, Font::Options fontOptions = {});
    void drawTextID(uint64_t id, const std::string& text, glm::vec2 pos, float height, const Options& options = {}, Font::Options fontOptions = {});

    /* uint32_t addShader(Shader*)
     *  returns: shader id
     *  parameters:
     *      - pointer to shader
     *
     *   Shaders added to renderer are automatically
     *   freed at end of Renderer lifetime
     */
    int addShader(Shader* shader, const std::string& name);
    int addTexture(Texture2D* texture, const std::string& name);
    Texture2D* getTexture(const std::string& name);
    Font* getFont(const std::string& name);

    glm::vec2 getViewportSize();
    glm::vec2 getCursorPos();

    void setEnforceLayer(bool enforceLayer);

private:
    struct BatchID {
        float layer = 0;
        uint32_t texture = 0;
        uint64_t shader = 0;
        uint64_t animation = 0;

        bool operator<(const BatchID& other) const {
            if(layer != other.layer) return layer < other.layer;
            if(texture != other.texture) return texture < other.texture;
            if(shader != other.shader) return shader < other.shader;
            return animation < other.animation;
        }
    };

    Window* _window{};

    std::map<BatchID, Batch*> _batches;
    std::map<std::string, Shader*> _shaders{};
    std::map<std::string, Texture2D*> _textures{};
    std::map<std::string, Font*> _fonts;

    bool _enforce_layer = false;

    Shader* get_shader(const std::string& name = "default");
    static float map_layer(float layer);
    static uint64_t get_batch_id(float layer, uint16_t texture, const std::string& shader);

    static void query_errors(const std::string& tag);
};


#endif //GOMOKU_RENDERER_H
