#include "presentation/renderer/animator/BackgroundAnimator.h"

#include <utility>
#include "common/Time.h"

BackgroundAnimator::BackgroundAnimator(double duration) : Animator(duration) {
}

BackgroundAnimator::BackgroundAnimator(double duration, float layer, std::string shader) : Animator(duration, layer, std::move(shader)) {
}

void BackgroundAnimator::update(Shader *shader) {
    Animator::update(shader);

    shader->uniform1lf("u_time", (float)_curr_time);
    //shader->uniform1lf("u_total_time", (float)Time::get()->getTime());
    if(_duration != 0)
        shader->uniform1lf("u_duration", (float)_duration);

    if(_pass_resolution)
        shader->uniform2fv("u_resolution", _renderer->getViewportSize());

    if(!_texture2.empty()){
        _renderer->getTexture(_texture2)->bind(1);
        shader->uniform1li("u_sampler2d2", 1);
    }
}

void BackgroundAnimator::setRenderer(Renderer *renderer) {
    _renderer = renderer;
}

void BackgroundAnimator::setTexture2(std::string texture2) {
    _texture2 = std::move(texture2);
}

void BackgroundAnimator::setPassResolution(bool passResolution) {
    _pass_resolution = passResolution;
}
