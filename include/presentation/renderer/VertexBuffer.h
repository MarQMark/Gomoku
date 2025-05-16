#ifndef GOMOKU_VERTEX_BUFFER_H
#define GOMOKU_VERTEX_BUFFER_H

#include <vector>
#include <memory>
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "VertexBufferLayout.h"

class VertexBuffer {
public:
    explicit VertexBuffer(VertexBufferLayout* vbl, unsigned int vertexSize);
    ~VertexBuffer();

    void addVertices(char* vertices, unsigned int count) const;


    void bind();

private:
    GLuint _id = 0;
    VertexBufferLayout* _vbl;

    unsigned int _vertex_size;

    GLint _offset = 0;
};

#endif //GOMOKU_VERTEX_BUFFER_H
