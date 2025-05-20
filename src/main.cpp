#include "presentation/renderer/Renderer.h"

int main() {

    Renderer renderer;


    while(renderer.shouldRun()){
        renderer.drawQuad(glm::vec2(0), glm::vec2(1), glm::vec4(.5), -1, 0);
        renderer.drawQuad(glm::vec2(0), glm::vec2(1), glm::vec4(0, .5, .8, 1), -1, 0);
        renderer.drawQuad(glm::vec2(-1, 1), glm::vec2(1.5), glm::vec4(.8, .5, 0, 1), 0, 0);
        renderer.render();
    }

    return 0;
}
