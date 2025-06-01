#include "presentation/renderer/animator/Animator.h"
#include "common/Time.h"

Animator::Animator(double duration) : _duration(duration) {
    _curr_time = _duration;
}

Animator::Animator(double duration, float layer, std::string shader) : _duration(duration), _layer(layer), _shader(std::move(shader)){
    _curr_time = _duration;
}

void Animator::update(Shader* shader) {
    if(!isActive())
        return;

    _curr_time += Time::get()->getDeltaTime();
}

void Animator::reset() {
    _curr_time = 0;
}

void Animator::setDuration(double duration) {
    _duration = duration;
}

bool Animator::isActive() const {
    return _curr_time < _duration || _duration == 0;
}

float Animator::getLayer() const {
    return _layer;
}

void Animator::setLayer(float layer) {
    _layer = layer;
}

const std::string &Animator::getShader() const {
    return _shader;
}

void Animator::setShader(const std::string &shader) {
    _shader = shader;
}

uint32_t Animator::getInstanceCount() const {
    return _instance_counter;
}

void Animator::addInstance() {
    _instance_counter++;
}

void Animator::removeInstance() {
    _instance_counter--;
}
