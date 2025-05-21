#include "presentation/renderer/Batch.h"
#include "Vertex.h"
#include <string.h>
#include <iostream>

Batch::Batch(Texture2D *texture2D, Shader *shader) :  _texture2D(texture2D), _shader(shader) {
    _vb = new VertexBuffer(Vertex::getLayout(), sizeof(Vertex));
}

Batch::~Batch() {
    for(auto pair : _buffers){
        if(pair.second->vb)
            free(pair.second->vb);
        if(pair.second->ib)
            free(pair.second->ib);
    }

    delete _vb;

    if(_ib)
        delete _ib;
}

void Batch::render() {
    // remove all unused vertices
    for (auto it = _buffers.begin(); it != _buffers.end(); ) {
        if (!it->second->used) {
            delete it->second;
            it = _buffers.erase(it);
            _dirty = true;
        } else {
            ++it;
        }
    }

    if(_dirty)
        recreate_buffers();

    if(!_shader || !_ib)
        return;

    _shader->bind();

    if(_texture2D){
        _texture2D->bind();
        _shader->uniform1li("u_sampler2d", 0);
    }

    _vb->bind();
    _ib->bind();

    glDrawElements(GL_TRIANGLES, _ib->getSize(), GL_UNSIGNED_INT, nullptr);

    for(auto pair : _buffers)
        pair.second->used = false;
}

void Batch::updateBuffer(uint64_t id, void *vb, uint32_t vbSize, uint32_t *ib, uint32_t ibSize) {
    if(!_buffers.count(id)) {
        _buffers[id] = new Buffer;
        _buffers[id]->vb = vb,
        _buffers[id]->vb_size = vbSize,
        _buffers[id]->ib = ib,
        _buffers[id]->ib_len = ibSize,
        _buffers[id]->used = true;
        _dirty = true;
        return;
    }

    Buffer* buffer = _buffers[id];
    if(vbSize != buffer->vb_size) {
        update_vb(buffer, vb, vbSize);
        return;
    }

    if(ibSize != buffer->ib_len) {
        update_ib(buffer, ib, ibSize);
        return;
    }

    if(memcmp(vb, buffer->vb, vbSize) != 0) {
        update_vb(buffer, vb, vbSize);
        return;
    }

    if(memcmp(ib, buffer->ib, ibSize) != 0) {
        update_ib(buffer, ib, ibSize);
        return;
    }

    buffer->used = true;
}

void Batch::update_vb(Buffer *buffer, void *vb, uint32_t vbSize) {
    free(buffer->vb);
    buffer->vb = vb;
    buffer->vb_size = vbSize;
    buffer->used = true;
    _dirty = true;
}

void Batch::update_ib(Buffer *buffer, uint32_t *ib, uint32_t ibSize) {
    free(buffer->ib);
    buffer->ib = ib;
    buffer->ib_len = ibSize;
    buffer->used = true;
    _dirty = true;
}

void Batch::recreate_buffers() {
    uint32_t vbSize = 0;
    uint32_t ibLen = 0;
    for (auto pair : _buffers) {
        vbSize += pair.second->vb_size;
        ibLen += pair.second->ib_len;
    }

    if(_buffers.empty() || vbSize == 0 || ibLen == 0)
        return;

    uint8_t* vertices = (uint8_t*)malloc(vbSize);
    uint32_t* indices = (uint32_t*)malloc(ibLen * sizeof(uint32_t));
    uint32_t vPos = 0;
    uint32_t iPos = 0;
    uint32_t iIdx = 0;
    for (auto pair : _buffers) {
        memcpy(&vertices[vPos], pair.second->vb, pair.second->vb_size);
        vPos += pair.second->vb_size;

        uint32_t highestIdx = 0;
        for(uint32_t i = 0; i < pair.second->ib_len; i++){
            highestIdx = std::max(highestIdx, pair.second->ib[i]);
            indices[iPos] = iIdx + pair.second->ib[i];
            iPos++;
        }
        iIdx += highestIdx + 1;
    }

    if(_ib)
        delete _ib;

    _ib = new IndexBuffer(indices, ibLen);
    _vb->addVertices((char*)vertices, vbSize / sizeof(Vertex));

    free(vertices);
    free(indices);

    _dirty = false;
}
