cmake_minimum_required(VERSION 3.17)
project(experiment1)

set(CMAKE_CXX_STANDARD 11)
include_directories(./include)
add_executable(experiment1 main.cpp glad.c)

target_link_libraries(experiment1 glfw3 glfw3 GL m Xrandr Xi X11 Xxf86vm pthread dl Xinerama Xcursor)