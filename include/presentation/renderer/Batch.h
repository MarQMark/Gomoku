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

    void updateBuffer(const std::string& name, void* vb, uint32_t vbLen, uint32_t* ib, uint32_t ibLen);

private:

    struct Buffer {
        void* vb{};
        uint32_t vb_len = 0;
        uint32_t* ib{};
        uint32_t ib_len = 0;
    };

    Texture2D* _texture2D{};
    Shader* _shader{};

    VertexBuffer* _vb{};
    IndexBuffer* _ib{};

    bool _dirty = false;
    std::map<std::string, Buffer*> _buffers{};

    void update_vb(Buffer* buffer, void* vb, uint32_t vbLen);
    void update_ib(Buffer* buffer, uint32_t* ib, uint32_t ibLen);
};

#endif //GOMOKU_BATCH_H
