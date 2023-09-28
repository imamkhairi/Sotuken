cmake_minimum_required(VERSION 3.0.0)
project(test_opengl VERSION 0.1.0)

# add_executable(test_opengl src/main.cpp /usr/include/glad/glad.h src/glad.c)

## window
add_executable(test_opengl src/window.cpp /usr/include/glad/glad.h src/glad.c)

## triangle
# add_executable(test_opengl src/triangle.cpp /usr/include/glad/glad.h src/glad.c)

## Index Buffer
# add_executable(test_opengl src/indexBuffer.cpp /usr/include/glad/glad.h src/glad.c)

## Organizing
# add_executable(test_opengl src/04organizing.cpp /usr/include/glad/glad.h src/glad.c)

target_link_libraries(test_opengl glfw)


# mungkin ini akan error karena ganti nama direktory