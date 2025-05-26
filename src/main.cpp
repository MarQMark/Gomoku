#include "presentation/renderer/Renderer.h"

#define STB_IMAGE_IMPLEMENTATION
#include "presentation/renderer/stb_image/stb_image.h"
#include "presentation/ui/UI.h"
#include "presentation/ui/Button.h"
#include "presentation/ui/Label.h"
#include "presentation/ui/Sprite.h"

void testBtnClbk(IInteractable* interactable, IInteractable::State state, void* data){
    std::cout << "Btn 1 Pressed" << "\n";
}

/* TODO:
 *  - Rework Shader access
 *  - Rework Texture management
 *  - Add Font access
 *  - Add layering stuff to IViewable/ Button...
 */

int main() {

    Renderer renderer;
    UI ui(&renderer);

    Button btn("testBtn", glm::vec2(0.25), glm::vec2(.25));
    btn.setKeepWidth(true);
    btn.registerCallback(testBtnClbk, IInteractable::State::PRESSED);
    ui.getViewable<View>("root")->addViewable(&btn);

    Button btn2("testBtn2", glm::vec2(0.5), glm::vec2(.25));
    //btn2.setKeepHeight(true);
    btn2.setAlignH(IViewable::Align::BOTTOM);
    btn2.setAlignV(IViewable::Align::RIGHT);
    btn2.setMargin(glm::vec4(0, 0.01, 0, 0.01));
    btn2.setTexture(IInteractable::State::NONE, 0);
    ui.getViewable<View>("root")->addViewable(&btn2);

    Label label("label", "This is a label");
    label.setDim(glm::vec2(0, 0.05));
    ui.getViewable<View>("root")->addViewable(&label);

    Sprite sprite("sprite", 0, glm::vec2(0, 0.5), glm::vec2(.25));
    ui.getViewable<View>("root")->addViewable(&sprite);

    int w, h, c;
    //stbi_set_flip_vertically_on_load(true);
    unsigned char* buf = stbi_load("/path", &w, &h, &c, 4);
    Texture2D* txt = new Texture2D(w, h, buf);
    std::cout << renderer.addTexture(txt) << "\n";


    while(renderer.shouldRun()){
        ui.update();
        renderer.render();
    }

    return 0;
}
