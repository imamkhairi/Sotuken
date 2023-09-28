cmake_minimum_required(VERSION 3.0.0)
project(test_opengl VERSION 0.1.0)

add_executable(test_opengl src/indexBuffer.cpp /usr/include/glad/glad.h src/glad.c)

target_link_libraries(test_opengl glfw)