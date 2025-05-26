#ifndef GOMOKU_BUTTON_H
#define GOMOKU_BUTTON_H

#include "IInteractable.h"

class Button : public IInteractable {
public:
    explicit Button(std::string name);
    Button(std::string name, glm::vec2 pos, glm::vec2 dim);

    void render(Renderer* renderer, glm::vec2 parentPos, glm::vec2 parentDim) override;

    void setTexture(State state, uint16_t textureID);
    void setShader(State state, uint16_t shaderID);

private:
    void init_maps();

    std::map<State, int32_t> _texture_ids;
    std::map<State, uint16_t> _shader_ids;
};


#endif //GOMOKU_BUTTON_H
