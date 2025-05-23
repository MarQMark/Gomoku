#ifndef GOMOKU_VIEW_H
#define GOMOKU_VIEW_H

#include <vector>
#include "IViewable.h"

class View : public IViewable{
public:
    View(std::string name);
    ~View() = default;

    void render(Renderer* renderer, glm::vec2 parentPos, glm::vec2 parentDim) override;

    void addViewable(IViewable* viewable);
    IViewable* getViewable(const std::string& name);

private:
    std::vector<IViewable*> _viewables{};

};


#endif //GOMOKU_VIEW_H
