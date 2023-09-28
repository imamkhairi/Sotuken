cmake_minimum_required(VERSION 3.0.0)
project(Sotuken VERSION 0.1.0)

include(CTest)
enable_testing()

find_package(OpenCV REQUIRED)
find_package(Threads REQUIRED)

# add_executable(Sotuken 02/noise.cc)
add_executable(Sotuken 02/point.cc)
# add_executable(Sotuken 02/block.cc)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

# set(CMAKE_CXX_STANDARD 11)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${OpenCV_INCLUDE_DIRS})
# C:\opencv\sources\include\opencv2

target_link_libraries(Sotuken ${OpenCV_LIBS} ${CMAKE_THREAD_LIBS_INIT})
#C:\opencv\build\x64\vc16\bin

# kalau mau compile ubuntu 
# g++ main.cpp -o main.o `pkg-config --cflags --libs opencv`