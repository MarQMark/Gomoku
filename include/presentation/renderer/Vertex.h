#ifndef GOMOKU_VERTEX_H
#define GOMOKU_VERTEX_H

#include "glm/glm.hpp"
#include "VertexBufferLayout.h"

struct Vertex{
    glm::vec3 position = glm::vec3(0);
    glm::vec4 color = glm::vec4(0);
    glm::vec2 texCoords = glm::vec3(0);
    glm::vec2 normal = glm::vec2(0);

    static VertexBufferLayout *getLayout() {
        auto layout = new VertexBufferLayout;

        layout->add<float>(3);
        layout->add<float>(4);
        layout->add<float>(2);
        layout->add<float>(2);

        return layout;
    }
};

#endif //GOMOKU_VERTEX_H
