#include "presentation/renderer/Animator.h"

Animator::Animator(double duration) : _duration(duration) {
    _last_time = glfwGetTime();
    _curr_time = _duration;
}

Animator::Animator(double duration, float layer, std::string shader) : _duration(duration), _layer(layer), _shader(std::move(shader)){
    _last_time = glfwGetTime();
    _curr_time = _duration;
}

void Animator::update(Shader* shader) {
    if(!isActive())
        return;

    shader->uniform1lf("u_time", (float)_curr_time);

    if(_duration != 0)
        shader->uniform1lf("u_duration", (float)_duration);

    _curr_time += glfwGetTime() - _last_time;
    _last_time = glfwGetTime();
}

void Animator::reset() {
    _curr_time = 0;
    _last_time = glfwGetTime();
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
