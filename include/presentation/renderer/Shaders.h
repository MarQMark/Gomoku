#ifndef GOMOKU_SHADERS_H
#define GOMOKU_SHADERS_H

#include <string>

struct StdShaders{
public:
    static const std::string sVS;
    static const std::string sFS;
};

const std::string StdShaders::sVS =
        "#version 430\n"
        "\n"
        "layout(location = 0) in vec4 position;\n"
        "layout(location = 1) in vec4 color;\n"
        "layout(location = 2) in vec2 texCoord;\n"
        "\n"
        "//uniform mat4 u_mvp;\n"
        "\n"
        "out vec4 v_color;\n"
        "out vec2 v_texCoord;\n"
        "out vec3 v_fragPos;\n"
        "\n"
        "void main() {\n"
        "    v_color = color;\n"
        "    v_texCoord = texCoord;\n"
        "    v_fragPos = vec3(position);\n"
        "    //gl_Position = u_mvp * position;\n"
        "    gl_Position =  position;\n"
        "}";

const std::string StdShaders::sFS =
        "#version 430 core\n"
        "\n"
        "in vec4 v_color;\n"
        "in vec2 v_texCoord;\n"
        "in vec3 v_fragPos;\n"
        "\n"
        "layout(location = 0) out vec4 fragColor;\n"
        "\n"
        "void main() {\n"
        "   fragColor = v_color;\n"
        "}";


#endif //GOMOKU_SHADERS_H
