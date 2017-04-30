
cmake_minimum_required(VERSION 3.5)

project(egl_headless)

add_executable(main main.c egl.c scene.c)
target_link_libraries(main GLEW GL EGL)
