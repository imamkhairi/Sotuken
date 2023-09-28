cmake_minimum_required(VERSION 3.0.0)
project(test_opengl VERSION 0.1.0)

add_executable(test_opengl src/04organizing.cpp /usr/include/glad/glad.h header/EBO.h header/VAO.h header/VBO.h header/shaderClass.h src/glad.c src/EBO.cpp src/shaderClass.cpp src/VAO.cpp src/VBO.cpp)

target_link_libraries(test_opengl glfw)

# add_executable(test_opengl src/indexBuffer.cpp /usr/include/glad/glad.h src/glad.c)



# mungkin ini akan error karena ganti nama direktory