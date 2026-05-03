cmake_minimum_required(VERSION 4.2)
project(raytracer_weekend_cpp)

set(CMAKE_CXX_STANDARD 23)

if (MSVC)
    add_compile_options(/utf-8)
endif()

add_executable(raytracer_weekend_cpp main.cpp)
target_include_directories(raytracer_weekend_cpp PRIVATE include)
