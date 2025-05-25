#ifndef GOMOKU_VIEWABLE_H
#define GOMOKU_VIEWABLE_H

#include <string>
#include "glm/glm.hpp"
#include "Renderer.h"

class IViewable{
public:

    bool isVisible(){return _visible;}
    void setVisible(bool visible) {_visible = visible;}

    std::string getName(){return _name;}

    glm::vec2 getPos(){return _pos;}
    void setPos(glm::vec2 pos){_pos = pos;}

    glm::vec2 getDim(){return _dim;}
    void setDim(glm::vec2 dim){_dim = dim;}

    glm::vec2 getAbsPos(){return _abs_pos;}
    glm::vec2 getAbsDim(){return _abs_dim;}

    virtual void render(Renderer* renderer, glm::vec2 parentPos, glm::vec2 parentDim) = 0;

protected:
    explicit IViewable(std::string name) : _name(std::move(name)) {}
    IViewable(std::string name, glm::vec2 pos, glm::vec2 dim) : _pos(pos), _dim(dim), _name(std::move(name)) {}
    ~IViewable() = default;

    glm::vec2 _pos = glm::vec2(0);
    glm::vec2 _dim = glm::vec2(1);

    glm::vec2 _abs_pos = glm::vec2(0);
    glm::vec2 _abs_dim = glm::vec2(0);

private:
    bool _visible = true;
    std::string _name;
};

#endif //GOMOKU_VIEWABLE_H
