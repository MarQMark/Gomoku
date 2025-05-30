#ifndef GOMOKU_ANIMATOR_H
#define GOMOKU_ANIMATOR_H


#include "Shader.h"

class Animator {
public:
    /* Animator(double)
     *     duration: time for one cycle of the animation
     *               0 means the animation if infinite and does not loop
     */
    explicit Animator(double duration = 0);

    virtual void clbk(Shader* shader);

    void setDuration(double duration);
    void reset();
    bool isActive() const;

private:
    double _duration = 0;
    double _curr_time = 0;
    double _last_time = 0;
};


#endif //GOMOKU_ANIMATOR_H
