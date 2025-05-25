#ifndef GOMOKU_FONT_H
#define GOMOKU_FONT_H

#include "presentation/renderer/Texture2D.h"
#include "glm/glm.hpp"
#include <map>

class Font {
public:
    struct Glyph{
        glm::vec2 offset = glm::vec2(0);
        glm::vec2 pos = glm::vec2(0);
        glm::vec2 dim = glm::vec2(0);
    };
    struct Options{
        Font* font;
        glm::vec4 color = glm::vec4(1);
        glm::vec2 spacing = glm::vec2(0.05f);
    };

public:
    explicit Font(void* data);
    ~Font();

    Glyph* getGlyph(char c);
    GLuint getID();
    Texture2D* getTexture();

private:
    typedef struct __attribute__((packed)) {
        uint32_t width;     // texture2D width
        uint32_t height;    // texture2D height
        uint32_t gCount;    // glyph count
        uint32_t gWidth;    // glyph max width
        uint32_t gHeight;   // glyph max height
    } Header;

    std::map<char, Glyph*> _glyphs;
    Texture2D* _txt;

    void parse_font(int8_t* data);
};

#endif //GOMOKU_FONT_H
