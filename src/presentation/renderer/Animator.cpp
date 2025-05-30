#include "presentation/renderer/Animator.h"

Animator::Animator(double duration) : _duration(duration) {
    _last_time = glfwGetTime();
    _curr_time = _duration;
}

void Animator::clbk(Shader* shader) {
    if(!isActive())
        return;

    shader->uniform1lf("u_time", (float)_curr_time);
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
    reset();
}

bool Animator::isActive() const {
    return _curr_time < _duration;
}
