#ifndef GOMOKU_BUTTON_H
#define GOMOKU_BUTTON_H

#include "IInteractable.h"

class Button : public IInteractable {
public:
    explicit Button(std::string name);
    Button(std::string name, glm::vec2 pos, glm::vec2 dim);

    void render(Renderer* renderer, glm::vec2 parentPos, glm::vec2 parentDim) override;

    void setTexture(uint16_t textureID);
    void setShader(uint16_t shaderID);

private:
    int32_t _texture_id = -1;
    uint16_t _shader_id = 0;
};


#endif //GOMOKU_BUTTON_H
