#ifndef GOMOKU_DRAWERVIEW_H
#define GOMOKU_DRAWERVIEW_H


#include "View.h"
#include "Button.h"

class DrawerView : public View{
private:
    class DrawerViewInteractable : public IInteractable {
    public:
        DrawerViewInteractable(std::string name);
        DrawerViewInteractable(std::string name, glm::vec2 pos, glm::vec2 dim);
        virtual ~DrawerViewInteractable() = default;

        void render(Renderer *renderer, glm::vec2 parentPos, glm::vec2 parentDim) override;

        void setHidden(bool hidden);

        void setTexture(std::string texture);
        void setShader(std::string shader);

    private:
        bool _hidden = false;

        float _layer = 0;
        std::string _texture{};
        std::string _shader{};
    };

public:
    explicit DrawerView(std::string name);
    DrawerView(std::string name, glm::vec2 pos, glm::vec2 dim);
    ~DrawerView() override;

    void render(Renderer *renderer, glm::vec2 parentPos, glm::vec2 parentDim) override;

    void addViewable(IViewable *viewable) override;

    void setExpanded(bool expanded);
    void setLeft(bool left);

    void setNavTogglePos(glm::vec2 pos);
    void setNavToggleDim(glm::vec2 dim);
    void setNavToggleLayer(float layer);
    void setNavToggleTexture(std::string texture);
    void setNavToggleShader(std::string shader);

private:
    glm::vec2 _nav_toggle_pos{};
    glm::vec2 _nav_toggle_dim{};
    DrawerViewInteractable* _nav_toggle{};

    std::vector<IViewable*> _viewables{};
    std::map<std::string, glm::vec2> _original_positions{};

    bool _left = true;
    bool _expanded = false;
    float _max_pos = 0;
    double _duration = .1;
    double _curr_time = 0;

    void init();
    void update_visibility();
    void set_interactable_small();
    void set_interactable_big();
};


#endif //GOMOKU_DRAWERVIEW_H
