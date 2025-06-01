#ifndef GOMOKU_VIEW_H
#define GOMOKU_VIEW_H

#include <vector>
#include "IViewable.h"

class View : public IViewable{
public:
    explicit View(std::string name);
    View(std::string name, glm::vec2 pos, glm::vec2 dim);
    virtual ~View() = default;

    void render(Renderer* renderer, glm::vec2 parentPos, glm::vec2 parentDim) override;

    virtual void addViewable(IViewable* viewable);

    template<class T = IViewable>
    T* getViewable(const std::string& name) {
        for(auto* viewable : _viewables){
            if(viewable->getName() == name){
                if(T* retVal =  dynamic_cast<T*>(viewable))
                    return retVal;
            }

            if(View* view = dynamic_cast<View*>(viewable)){
                T* retVal = view->getViewable<T>(name);
                if(retVal)
                    return retVal;
            }
        }

        return nullptr;
    };

    std::vector<IViewable*> getViewables();
    void getChildViewables(std::vector<IViewable*>& viewables);

private:
    std::vector<IViewable*> _viewables{};
};


#endif //GOMOKU_VIEW_H
