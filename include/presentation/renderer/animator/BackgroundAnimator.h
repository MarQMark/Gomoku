#ifndef GOMOKU_BACKGROUNDANIMATOR_H
#define GOMOKU_BACKGROUNDANIMATOR_H


#include "Animator.h"
#include "Renderer.h"

class BackgroundAnimator : public Animator{
public:
    explicit BackgroundAnimator(double duration = 0);
    BackgroundAnimator(double duration, float layer, std::string shader);

    void update(Shader *shader) override;

    void addRenderer(Renderer* renderer);

private:
    Renderer* _renderer{};
};


#endif //GOMOKU_BACKGROUNDANIMATOR_H
