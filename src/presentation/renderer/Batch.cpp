#include "presentation/renderer/Batch.h"
#include "presentation/renderer/buffer/Vertex.h"
#include <iostream>
#include <cstring>

Batch::Batch(Texture2D *texture2D, Shader *shader) :  _texture2D(texture2D), _shader(shader) {
    _vb = new VertexBuffer(Vertex::getLayout(), sizeof(Vertex));
}

Batch::~Batch() {
    for(auto pair : _buffers){
        delete pair.second;
    }

    delete _vb;
    delete _ib;
}

int Batch::render() {
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

    if(_dirty){
        // Returns false if Batch is empty
        if(recreate_buffers() != 1)
            return -1;
    }

    if(!_shader)
        return -2;
    if(!_ib)
        return -3;

    _shader->bind();
    if(_animator)
        _animator->update(_shader);

    if(_texture2D){
        _texture2D->bind();
        _shader->uniform1li("u_sampler2d", 0);
    }

    _vb->bind();
    _ib->bind();

    glDrawElements(GL_TRIANGLES, _ib->getSize(), GL_UNSIGNED_INT, nullptr);

    for(auto pair : _buffers)
        pair.second->used = false;

    return 1;
}

void Batch::updateBuffer(uint64_t id, void *vb, uint32_t vbSize, uint32_t *ib, uint32_t ibSize) {
    if(!_buffers.count(id)) {
        _buffers[id] = new Buffer;
        _buffers[id]->vb = vb,
        _buffers[id]->vbSize = vbSize,
        _buffers[id]->ib = ib,
        _buffers[id]->ibLen = ibSize,
        _buffers[id]->used = true;
        _dirty = true;
        return;
    }

    Buffer* buffer = _buffers[id];
    bool vbUpdate = false, ibUpdate = false;
    if(vbSize != buffer->vbSize || memcmp(vb, buffer->vb, vbSize) != 0) {
        update_vb(buffer, vb, vbSize);
        vbUpdate = true;
    }

    if(ibSize != buffer->ibLen || memcmp(ib, buffer->ib, ibSize) != 0) {
        update_ib(buffer, ib, ibSize);
        ibUpdate = true;
    }

    if(!vbUpdate)
        free(vb);
    if(!ibUpdate)
        free(ib);
    buffer->used = true;
}

void Batch::update_vb(Buffer *buffer, void *vb, uint32_t vbSize) {
    free(buffer->vb);
    buffer->vb = vb;
    buffer->vbSize = vbSize;
    buffer->used = true;
    _dirty = true;
}

void Batch::update_ib(Buffer *buffer, uint32_t *ib, uint32_t ibSize) {
    free(buffer->ib);
    buffer->ib = ib;
    buffer->ibLen = ibSize;
    buffer->used = true;
    _dirty = true;
}

int Batch::recreate_buffers() {
    uint32_t vbSize = 0;
    uint32_t ibLen = 0;
    for (auto pair : _buffers) {
        vbSize += pair.second->vbSize;
        ibLen += pair.second->ibLen;
    }

    // Return if all buffers are empty i.e. complete batch is empty => nothing to render
    if(_buffers.empty() || vbSize == 0 || ibLen == 0)
        return -1;

    uint8_t* vertices = (uint8_t*)malloc(vbSize);
    uint32_t* indices = (uint32_t*)malloc(ibLen * sizeof(uint32_t));
    uint32_t vPos = 0;
    uint32_t iPos = 0;
    uint32_t iIdx = 0;
    for (auto pair : _buffers) {
        memcpy(&vertices[vPos], pair.second->vb, pair.second->vbSize);
        vPos += pair.second->vbSize;

        uint32_t highestIdx = 0;
        for(uint32_t i = 0; i < pair.second->ibLen; i++){
            highestIdx = std::max(highestIdx, pair.second->ib[i]);
            indices[iPos] = iIdx + pair.second->ib[i];
            iPos++;
        }
        iIdx += highestIdx + 1;
    }

    delete _ib;

    _ib = new IndexBuffer(indices, ibLen);
    _vb->addVertices((char*)vertices, vbSize / sizeof(Vertex));

    free(vertices);
    free(indices);

    _dirty = false;

    return 1;
}

void Batch::setAnimator(Animator *animator) {
    _animator = animator;
}
