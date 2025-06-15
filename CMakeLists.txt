cmake_minimum_required(VERSION 3.15)
project(GameMath)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

foreach(dir
    01Line
    02Ortho
    03ObjectToWorld
    04Mouse
    05Perspective
    06BackfaceCulling
    07Camera
    08Rasterizer
    09Barycentric
    10NormalInterpolation
    11Texture
    12DirectX11
)
    add_subdirectory(${dir})
endforeach()
