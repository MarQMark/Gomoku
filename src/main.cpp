#include "presentation/renderer/Renderer.h"

int main() {

    Renderer renderer;

    double time = glfwGetTime();
    float pos = -1;

    while(renderer.shouldRun()){
        renderer.drawQuad(glm::vec2(0), glm::vec2(1), glm::vec4(0, .5, (std::sin(glfwGetTime()) + 1) / 2, 1), -1);

        if(int(glfwGetTime() - time) % 4 < 2)
            renderer.drawQuad(glm::vec2(pos, 1), glm::vec2(1.5), glm::vec4(0.2, .5, 0, 1), 0);

        if(int(glfwGetTime() - time) % 4 == 0)
            pos += 0.005f;
        else if(int(glfwGetTime() - time) % 4 == 1)
            pos -= 0.005f;

        renderer.render();
    }

    return 0;
}
