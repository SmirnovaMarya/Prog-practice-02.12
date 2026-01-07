cmake_minimum_required(VERSION 4.1)
project(untitled)

set(CMAKE_CXX_STANDARD 11)

add_executable(intitled
    figures/main.cpp
    figures/geom.cpp
    figures/canvas.cpp
    figures/idraw.cpp
    figures/dot.cpp
    figures/rect.cpp
    figures/frect.cpp
    figures/hline.cpp
    figures/square.cpp
)
