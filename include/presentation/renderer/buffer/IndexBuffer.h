#ifndef GOMOKU_INDEX_BUFFER_H
#define GOMOKU_INDEX_BUFFER_H

#include <vector>
#include "GL/glew.h"

class IndexBuffer {
public:
    explicit IndexBuffer(std::vector<GLuint>& indices);
    explicit IndexBuffer(uint32_t* indices, uint32_t size);
    ~IndexBuffer();

    void bind() const;
    static void unbind();

    uint32_t getSize() const;

private:
    GLuint _id = 0;
    uint32_t _size = 0;
};

#endif //GOMOKU_INDEX_BUFFER_H
