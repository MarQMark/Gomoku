#ifndef GOMOKU_SHADERSPRITE_H
#define GOMOKU_SHADERSPRITE_H


#include "IViewable.h"

class ShaderSprite : public IViewable{
public:
    ShaderSprite(std::string name, std::string shaderName);
    ShaderSprite(std::string name, std::string shaderName, glm::vec2 pos, glm::vec2 dim);
    ~ShaderSprite();

    void render(Renderer* renderer, glm::vec2 parentPos, glm::vec2 parentDim) override;

    std::string getShader();
    void setShader(std::string shader);

    Animator* getAnimator();
    void setAnimator(Animator* animator);

private:
    std::string _shader_name{};
    Animator* _animator{};
};


#endif //GOMOKU_SHADERSPRITE_H
