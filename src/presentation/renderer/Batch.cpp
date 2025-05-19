#include "presentation/renderer/Batch.h"

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
