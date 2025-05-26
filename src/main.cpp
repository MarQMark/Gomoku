#include "presentation/renderer/Renderer.h"

#define STB_IMAGE_IMPLEMENTATION
#include "presentation/renderer/stb_image/stb_image.h"
#include "presentation/ui/UI.h"
#include "presentation/ui/Button.h"

void testBtnClbk(IInteractable* interactable, IInteractable::State state, void* data){
    std::cout << "Btn 1 Pressed" << "\n";
}

int main() {

    Renderer renderer;
    UI ui(&renderer);

    Button btn("testBtn", glm::vec2(0.25), glm::vec2(.25));
    btn.registerCallback(testBtnClbk, IInteractable::State::PRESSED);
    ui.getViewable<View>("root")->addViewable(&btn);
    Button btn2("testBtn2", glm::vec2(0.5), glm::vec2(.25));
    ui.getViewable<View>("root")->addViewable(&btn2);

    double time = glfwGetTime();
    float pos = -1;


    int w, h, c;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* buf = stbi_load("test", &w, &h, &c, 4);
    Texture2D* txt = new Texture2D(w, h, buf);
    std::cout << renderer.addTexture(txt) << "\n";


    while(renderer.shouldRun()){
        //renderer.drawQuad(glm::vec2(0), glm::vec2(1), glm::vec4(0, .5, (std::sin(glfwGetTime()) + 1) / 2, 1), -2);

        //renderer.drawQuad(glm::vec2(-1, .5), glm::vec2(1.5), glm::vec4(0, .5, .9, 1), 1);
        renderer.drawTexture(glm::vec2(-1), glm::vec2(1.5), 0, -1);

        renderer.drawText("Yay this finally works", glm::vec2(0), .05, 7, 2);

        //if(int(glfwGetTime() - time) % 4 < 2)
        //    renderer.drawQuad(glm::vec2(pos, 1), glm::vec2(1.5), glm::vec4(0.2, .5, 0, 1), 0);

        if(int(glfwGetTime() - time) % 4 == 0)
            pos += 0.005f;
        else if(int(glfwGetTime() - time) % 4 == 1)
            pos -= 0.005f;

        //renderer.drawQuad(glm::vec2(-.5, .8), glm::vec2(1, 1.78f), glm::vec4(0), 4, 1);
       // renderer.drawQuad(glm::vec2(-.5, .5), glm::vec2(1), glm::vec4(0), 4, 1);

        ui.update();
        renderer.render();
    }

    return 0;
}
