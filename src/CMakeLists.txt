project(Gomoku)

add_executable(Gomoku

        presentation/renderer/Window.cpp

        main.cpp
)

target_include_directories(Gomoku
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../external/glfw/include
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