#include "presentation/renderer/animator/SimpleAnimator.h"

#include <utility>

SimpleAnimator::SimpleAnimator(double duration) : Animator(duration) {
}
SimpleAnimator::SimpleAnimator(double duration, float layer, std::string shader) : Animator(duration, layer, std::move(shader)) {
}

void SimpleAnimator::update(Shader *shader) {
    Animator::update(shader);

    shader->uniform1lf("u_time", (float)_curr_time);
    if(_duration != 0)
        shader->uniform1lf("u_duration", (float)_duration);
}
