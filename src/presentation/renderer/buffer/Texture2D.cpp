#include "presentation/renderer/buffer/Texture2D.h"

Texture2D::Texture2D(GLsizei width, GLsizei height, uint8_t *data, Texture2D::Options *opt) : _width(width), _height(height){
    bool freeOpt = false;
    if(!opt){
        freeOpt = true;
        opt = new Options();
    }

    glGenTextures(1, &_id);
    bind();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, opt->minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, opt->maxFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, opt->warpS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, opt->warpT);

    glTexImage2D(GL_TEXTURE_2D, opt->level, opt->internalformat, width, height, 0, opt->format, opt->type, data);
    unbind();

    if(freeOpt)
        delete opt;
}

Texture2D::~Texture2D() {
    glDeleteTextures(1, &_id);
}

void Texture2D::bind(GLuint slot) const {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, _id);
}

void Texture2D::unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}

GLuint Texture2D::get() const {
    return _id;
}

GLsizei Texture2D::getWidth() const {
    return _width;
}

GLsizei Texture2D::getHeight() const {
    return _height;
}
