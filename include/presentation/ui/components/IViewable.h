#ifndef GOMOKU_VIEWABLE_H
#define GOMOKU_VIEWABLE_H

#include <string>
#include "glm/glm.hpp"
#include "Renderer.h"

class IViewable{
public:

    enum Align{
        NONE = 0,
        CENTER = 1,
        TOP = 2,
        BOTTOM = 3,
        LEFT = 2,
        RIGHT = 3,
    };

    virtual bool isVisible() const {return _visible;}
    virtual void setVisible(bool visible) {_visible = visible;}

    virtual std::string getName(){return _name;}

    virtual glm::vec2 getPos(){return _pos;}
    virtual void setPos(glm::vec2 pos){_pos = pos;}

    virtual glm::vec2 getDim(){return _dim;}
    virtual void setDim(glm::vec2 dim){_dim = dim;}

    virtual glm::vec2 getAbsPos(){return _abs_pos;}
    virtual glm::vec2 getAbsDim(){return _abs_dim;}

    virtual void setKeepWidth(bool keep){_keep_width = keep;}
    virtual void setKeepHeight(bool keep){_keep_height = keep;}

    /* void setMargin(glm::vec4)
     *  The components correspond as follows and are applied in the same order:
     *      x: top
     *      y: bottom
     *      z: left
     *      w: right
     */
    virtual void setMargin(glm::vec4 margin){_margin = margin;}
    virtual void setAlignV(Align align){_vertical = align;}
    virtual void setAlignH(Align align){_horizontal = align;}

    virtual void setLayer(float layer){_layer = layer;}
    virtual float getLayer() const{ return _layer;}

    virtual void render(Renderer* renderer, glm::vec2 parentPos, glm::vec2 parentDim) = 0;

protected:
    explicit IViewable(std::string name) : _name(std::move(name)) {}
    IViewable(std::string name, glm::vec2 pos, glm::vec2 dim) : _pos(pos), _dim(dim), _name(std::move(name)) {}
    ~IViewable() = default;

    glm::vec2 _pos = glm::vec2(0);
    glm::vec2 _dim = glm::vec2(1);

    glm::vec2 _abs_pos = glm::vec2(0);
    glm::vec2 _abs_dim = glm::vec2(0);

    bool _keep_width = false;
    bool _keep_height = false;

    Align _vertical = Align::NONE;
    Align _horizontal = Align::NONE;
    glm::vec4 _margin = glm::vec4(0);

    float _layer = 0;

private:
    bool _visible = true;
    std::string _name;
};

#endif //GOMOKU_VIEWABLE_H
