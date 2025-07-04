#ifndef GOMOKU_SHADER_H
#define GOMOKU_SHADER_H

#include <string>
#include <map>
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

class Shader {
public:
    Shader(const std::string &vertexPath, const std::string &fragmentPath);

    ~Shader();

    void bind() const;
    static void unbind();

    static std::string loadShaderSource(const std::string &path);

    void uniform1li(const std::string &name, int x);
    void uniform1iv(const std::string &name, GLsizei count, const GLint* value);
    void uniform1lf(const std::string &name, float x);
    void uniform2fv(const std::string &name, glm::vec2 v);
    void uniform3fv(const std::string &name, glm::vec3 v);
    void uniform4fv(const std::string &name, glm::vec4 v);
    void uniformM4fv(const std::string &name, glm::mat4x4 mvp);
private:
    static int compile_shader(GLenum type, const std::string &source);
    void create_program(GLuint vs, GLuint fs);

    int uniform_location(const std::string &name);

    GLuint _id;
    GLuint _vs;
    GLuint _fs;
    std::map<std::string, GLint> _uniforms;
};

#endif //GOMOKU_SHADER_H
