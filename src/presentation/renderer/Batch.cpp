#include "presentation/renderer/Batch.h"
#include <string.h>

Batch::Batch(Texture2D *texture2D, Shader *shader) :  _texture2D(texture2D), _shader(shader) {
}

void Batch::render() {
    if(_shader)
        _shader->bind();

    if(_texture2D)
        _texture2D->bind();

    _vb->bind();
    _ib->bind();

    glDrawElements(GL_TRIANGLES, _ib->getSize(), GL_UNSIGNED_INT, nullptr);
}

void Batch::updateBuffer(const std::string& name, void *vb, uint32_t vbLen, uint32_t *ib, uint32_t ibLen) {
    if(!_buffers.count(name)) {
        _buffers[name] = new Buffer{
            .vb = vb,
            .vb_len = vbLen,
            .ib = ib,
            .ib_len = ibLen
        };
        _dirty = true;
        return;
    }

    Buffer* buffer = _buffers[name];
    if(vbLen != buffer->vb_len) {
        update_vb(buffer, vb, vbLen);
        return;
    }

    if(ibLen != buffer->ib_len) {
        update_ib(buffer, ib, ibLen);
        return;
    }

    if(memcmp(vb, buffer->vb, vbLen) != 0) {
        update_vb(buffer, vb, vbLen);
        return;
    }

    if(memcmp(ib, buffer->ib, vbLen) != 0) {
        update_ib(buffer, ib, ibLen);
        return;
    }
}

void Batch::update_vb(Buffer *buffer, void *vb, uint32_t vbLen) {
    free(buffer->vb);
    buffer->vb = vb;
    buffer->vb_len = vbLen;
    _dirty = true;
}

void Batch::update_ib(Buffer *buffer, uint32_t *ib, uint32_t ibLen) {
    free(buffer->ib);
    buffer->ib = ib;
    buffer->ib_len = ibLen;
    _dirty = true;
}
