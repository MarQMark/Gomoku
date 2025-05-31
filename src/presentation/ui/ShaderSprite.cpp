#include "presentation/ui/ShaderSprite.h"

#include <utility>

ShaderSprite::ShaderSprite(std::string name, std::string shaderName)  : IViewable(std::move(name)), _shader_name(std::move(shaderName)){
    _layer = 3;
}

ShaderSprite::ShaderSprite(std::string name, std::string shaderName, glm::vec2 pos, glm::vec2 dim) :
        IViewable(std::move(name), pos, dim), _shader_name(std::move(shaderName)){
    _layer = 4;
}

ShaderSprite::~ShaderSprite() {
    delete _animator;
}

void ShaderSprite::render(Renderer *renderer, const glm::vec2 parentPos, const glm::vec2 parentDim) {
    IViewable::render(renderer, parentPos, parentDim);
    if(!isVisible())
        return;

    Renderer::Options options;
    options.shaderName = _shader_name;
    options.layer = _layer;
    if(_animator && _animator->isActive()){
        options.shaderName = _animator->getShader();
        options.animator = _animator;
        options.animationID = (uint64_t)this;
        options.layer = _animator->getLayer();
    }
    renderer->drawQuadID((uint64_t)this, _abs_pos, _abs_dim, glm::vec4(1), options);
}

Animator *ShaderSprite::getAnimator() {
    return _animator;
}

void ShaderSprite::setAnimator(Animator *animator) {
    _animator = animator;
}

std::string ShaderSprite::getShader() {
    return _shader_name;
}

void ShaderSprite::setShader(std::string shader) {
    _shader_name = std::move(shader);
}
