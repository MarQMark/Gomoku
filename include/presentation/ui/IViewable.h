#ifndef GOMOKU_VIEWABLE_H
#define GOMOKU_VIEWABLE_H

#include <string>
#include "glm/glm.hpp"

class IViewable{
public:
    glm::vec2 pos = glm::vec2(0);
    glm::vec2 dim = glm::vec2(0);

    void setVisible(bool visible) {
        _visible = visible;
    }
    bool isVisible(){
        return _visible;
    }

    std::string getName(){
        return _name;
    }

    virtual void render() = 0;

protected:
    IViewable(std::string name) : _name(std::move(name)) {}
    ~IViewable() = default;

private:
    bool _visible = true;
    std::string _name;
};

#endif //GOMOKU_VIEWABLE_H
