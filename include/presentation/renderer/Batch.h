#ifndef GOMOKU_BATCH_H
#define GOMOKU_BATCH_H

#include "Shader.h"
#include "Texture2D.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

class Batch {
public:
    Batch(Texture2D* texture2D = nullptr, Shader* shader = nullptr);
    ~Batch() = default;

    void render();

private:
    Texture2D* _texture2D{};
    Shader* _shader{};

    VertexBuffer* _vb{};
    IndexBuffer* _ib{};
};

#endif //GOMOKU_BATCH_H
