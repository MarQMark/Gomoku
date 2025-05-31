#ifndef GOMOKU_SIMPLEANIMATOR_H
#define GOMOKU_SIMPLEANIMATOR_H


#include "Animator.h"

class SimpleAnimator : public Animator {
public:
    explicit SimpleAnimator(double duration = 0);
    SimpleAnimator(double duration, float layer, std::string shader);

    void update(Shader *shader) override;

private:
};


#endif //GOMOKU_SIMPLEANIMATOR_H
