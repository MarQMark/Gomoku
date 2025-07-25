#ifndef GOMOKU_BACKGROUNDANIMATOR_H
#define GOMOKU_BACKGROUNDANIMATOR_H


#include "Animator.h"
#include "Renderer.h"

class BackgroundAnimator : public Animator{
public:
    explicit BackgroundAnimator(double duration = 0);
    BackgroundAnimator(double duration, float layer, std::string shader);

    void update(Shader *shader) override;

    void setRenderer(Renderer* renderer);
    void setTexture2(std::string texture2);

    void setPassResolution(bool passResolution);

private:
    Renderer* _renderer{};
    std::string _texture2{};

    bool _pass_resolution = true;
};


#endif //GOMOKU_BACKGROUNDANIMATOR_H
