#include "presentation/renderer/animator/BackgroundAnimator.h"

BackgroundAnimator::BackgroundAnimator(double duration) : Animator(duration) {
}

BackgroundAnimator::BackgroundAnimator(double duration, float layer, std::string shader) : Animator(duration, layer, shader) {
}

void BackgroundAnimator::update(Shader *shader) {
    Animator::update(shader);

    shader->uniform1lf("u_time", (float)_curr_time);
    if(_duration != 0)
        shader->uniform1lf("u_duration", (float)_duration);

    shader->uniform2fv("u_resolution", _renderer->getViewportSize());
}

void BackgroundAnimator::addRenderer(Renderer *renderer) {
    _renderer = renderer;
}
