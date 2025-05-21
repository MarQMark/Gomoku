#ifndef GOMOKU_VIEW_H
#define GOMOKU_VIEW_H

#include <vector>
#include "IViewable.h"

class View {
public:
    View() = default;
    ~View() = default;

    void update();

    void addView(View* view);
    void addViewable(IViewable* viewable);

private:
    std::vector<View*> _views{};
    std::vector<IViewable*> _viewables{};
};


#endif //GOMOKU_VIEW_H
