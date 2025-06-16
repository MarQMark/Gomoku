#ifndef GOMOKU_BATCH_H
#define GOMOKU_BATCH_H

#include "Shader.h"
#include "presentation/renderer/buffer/Texture2D.h"
#include "presentation/renderer/buffer/VertexBuffer.h"
#include "presentation/renderer/buffer/IndexBuffer.h"
#include "presentation/renderer/animator/Animator.h"

class Batch {
public:
    Batch(Texture2D* texture2D = nullptr, Shader* shader = nullptr);
    ~Batch();

    /* Returns Values:
     *   1 -> Success
     *  -1 -> Batch is empty
     *  -2 -> Invalid Shader
     *  -3 -> Invalid Index Buffer
     *
     */
    int render();

    void updateBuffer(uint64_t id, void* vb, uint32_t vbSize, uint32_t* ib, uint32_t ibSize);
    void setAnimator(Animator* animator);

private:

    struct Buffer {
        ~Buffer(){
            free(vb);
            free(ib);
        }

        void* vb{};
        uint32_t vbSize = 0;
        uint32_t* ib{};
        uint32_t ibLen = 0;
        bool used = true;
    };

    Texture2D* _texture2D{};
    Shader* _shader{};
    Animator* _animator{};

    VertexBuffer* _vb{};
    IndexBuffer* _ib{};

    std::map<uint64_t, Buffer*> _buffers{};

    void update_vb(Buffer* buffer, void* vb, uint32_t vbSize);
    void update_ib(Buffer* buffer, uint32_t* ib, uint32_t ibSize);

    bool _dirty = true;
    int recreate_buffers();
};

#endif //GOMOKU_BATCH_H
