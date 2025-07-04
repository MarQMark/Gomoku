project(Gomoku)

add_executable(Gomoku
        presentation/renderer/glew/glew.c

        presentation/renderer/Window.cpp
        presentation/renderer/Renderer.cpp
        presentation/renderer/Shader.cpp
        presentation/renderer/Batch.cpp
        presentation/renderer/Font.cpp
        presentation/renderer/buffer/VertexBuffer.cpp
        presentation/renderer/buffer/IndexBuffer.cpp
        presentation/renderer/buffer/Texture2D.cpp
        presentation/renderer/animator/Animator.cpp
        presentation/renderer/animator/SimpleAnimator.cpp
        presentation/renderer/animator/BackgroundAnimator.cpp

        presentation/input/Input.cpp

        presentation/ui/components/Viewable.cpp
        presentation/ui/components/View.cpp
        presentation/ui/components/DrawerView.cpp
        presentation/ui/components/Label.cpp
        presentation/ui/components/Sprite.cpp
        presentation/ui/components/ShaderSprite.cpp
        presentation/ui/components/Button.cpp
        presentation/ui/components/MenuView.cpp
        presentation/ui/UI.cpp
        presentation/ui/BackgroundView.cpp
        presentation/ui/MenuButton.cpp
        presentation/ui/MenuController.cpp
        presentation/ui/GameMenuView.cpp
        presentation/ui/MainMenuView.cpp
        presentation/ui/ModeMenuView.cpp
        presentation/ui/DifficultyMenuView.cpp
        presentation/ui/StatsMenuView.cpp
        presentation/ui/BoardView.cpp

        presentation/assets/AssetManager.cpp

        presentation/mapping/MapViewToCommand.cpp

        logic/mapping/MapLogicToView.cpp
        logic/Board.cpp
        logic/GameService.cpp
        logic/player/HumanPlayer.cpp
        logic/player/AIPlayer.cpp
        logic/mapping/MapLogicToView.cpp
        logic/mapping/MapLogicToModel.cpp

        common/Time.cpp
        common/RunManager.cpp

        persistence/FileManager.cpp
        persistence/mapping/MapPersistenceToLogic.cpp

        Game.cpp
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
add_compile_definitions(STB_IMAGE_IMPLEMENTATION)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(DEBUG)

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
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_definitions(USE_EMBEDDED)

    include(embed_res.cmake)
    embed_assets_as_c_arrays(Gomoku "${CMAKE_SOURCE_DIR}/res")

    include(embed_shaders.cmake)
    embed_shaders_as_c_arrays(Gomoku "${CMAKE_SOURCE_DIR}/shaders")
endif()


if (UNIX)
    find_package(OpenGL REQUIRED)
    target_link_libraries(Gomoku glut GL dl X11 pthread)
endif(UNIX)

if (WIN32)
    target_link_libraries(Gomoku opengl32)
endif(WIN32)