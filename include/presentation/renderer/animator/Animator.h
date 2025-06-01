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
    Animator(double duration, float layer, std::string shader);
    virtual ~Animator() = default;

    virtual void update(Shader* shader);

    void setDuration(double duration);
    void reset();
    bool isActive() const;

    float getLayer() const;
    void setLayer(float layer);
    const std::string &getShader() const;
    void setShader(const std::string &shader);

    uint32_t getInstanceCount() const;
    void addInstance();
    void removeInstance();

protected:
    double _duration = 0;
    double _curr_time = 0;

    float _layer = 0;
    std::string _shader{};

private:
    uint32_t _instance_counter = 0;
};


#endif //GOMOKU_ANIMATOR_H
