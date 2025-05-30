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
        presentation/renderer/Font.cpp
        presentation/renderer/Animator.cpp

        presentation/input/Input.cpp

        presentation/ui/UI.cpp
        presentation/ui/Viewable.cpp
        presentation/ui/View.cpp
        presentation/ui/Label.cpp
        presentation/ui/Sprite.cpp
        presentation/ui/Button.cpp
        presentation/ui/BoardView.cpp

        presentation/assets/AssetManager.cpp
        presentation/assets/AssetManager.h

        presentation/mapping/MapPresentationToCommand.cpp
        presentation/mapping/MapPresentationToCommand.h

        logic/mapping/MapLogicToView.cpp
        logic/Board.cpp
        logic/GameService.cpp

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

add_compile_definitions(GLEW_STATIC)

add_custom_target(copy_shaders ALL
        COMMAND  ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/../shaders
        ${PROJECT_BINARY_DIR}/shaders        COMMENT "Copying shaders into binary directory")
add_dependencies(Gomoku copy_shaders)

add_custom_target(copy_resources ALL
        COMMAND  ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/../res
        ${PROJECT_BINARY_DIR}/res        COMMENT "Copying resources into binary directory")
add_dependencies(Gomoku copy_resources)

if (UNIX)
    find_package(OpenGL REQUIRED)
    target_link_libraries(Gomoku glut GL dl X11 pthread)
endif(UNIX)

if (WIN32)
    target_link_libraries(Gomoku opengl32)
endif(WIN32)