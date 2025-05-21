project(Gomoku)

add_executable(Gomoku
        presentation/renderer/glew/glew.c

        presentation/renderer/Window.cpp
        presentation/renderer/Renderer.cpp
        presentation/renderer/VertexBuffer.cpp
        presentation/renderer/IndexBuffer.cpp
        presentation/renderer/Texture2D.cpp
        presentation/renderer/Shader.cpp
        presentation/renderer/Batch.cpp

        presentation/ui/UI.cpp
        presentation/ui/Button.cpp
        presentation/ui/View.cpp

        main.cpp
)

target_include_directories(Gomoku
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include/presentation/renderer/
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../external/glfw/include
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../external/glm/
)

target_link_directories(${PROJECT_NAME} PRIVATE external/glfw/src)

target_link_libraries(Gomoku glfw)

if (UNIX)
    find_package(OpenGL REQUIRED)
    target_link_libraries(Gomoku glut GL dl X11 pthread)
endif(UNIX)

if (WIN32)
    target_link_libraries(Gomoku opengl32)
endif(WIN32)